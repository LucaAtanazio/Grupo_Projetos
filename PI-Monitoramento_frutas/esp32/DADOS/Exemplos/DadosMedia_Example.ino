#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#include "config.h"

// =========================================
// CONFIGURAÇÃO DO WI-FI
// =========================================
const char* ssid = "NOME_DA_REDE";
const char* password = "NOME_DA_REDE";
// =========================================
// CONFIGURAÇÃO DOS SENSORES
// =========================================
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define MQ3_PIN 34   // Sensor MQ-3

// =========================================
// VARIÁVEIS CONFIGURÁVEIS
// =========================================
String tipoFruta = "banana_terra";
String lote = "lote_[teste_banana_passada]"; // Mudar a cada monitoramento
String estado_real = "sem risco"; // Mudar a cada monitoramento = [ Sem risco - Madura - Alerda - Risco de perda ]

// =========================================
// ENDPOINT DO SERVIDOR FLASK (VERIFIQUE O IP!)
// =========================================
// Se o seu IP mudar, atualize esta linha para o IP real da sua máquina.
String serverURL = "http://IP:8080/api/sensores"; 
// =========================================
// RECONEXÃO MÉDIA
// =========================================
void reconectaWiFi() {
    if (WiFi.status() == WL_CONNECTED) return;

    Serial.println("WiFi desconectado! Tentando reconectar...");
    int tentativas = 0;
    WiFi.disconnect(true);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && tentativas < 5) {
        delay(800);
        Serial.print(".");
        tentativas++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi reconectado.");
    } else {
        Serial.println("\nFalha ao reconectar. Tentará no próximo ciclo.");
    }
}

// =========================================
// Função: EMA + 100 leituras em 3,75 min
// =========================================
int readMQ3_EMA(int pin) {

    const int totalLeituras = 100;
    // agora 100 leituras
    const int intervalo = 2250;
    // 225000 ms → 3.75 leituras ≈ 75% de 5 min
    const float alpha = 0.05;
    // suavização do EMA (5%)

    float ema = analogRead(pin);
    // inicializa EMA

    for (int i = 1; i < totalLeituras; i++) {
        int leitura = analogRead(pin);
        ema = alpha * leitura + (1 - alpha) * ema;
        delay(intervalo);
    }

    return (int)ema;
}

// =========================================
// Setup
// =========================================
void setup() {
    Serial.begin(115200);
    delay(3000);

    WiFi.begin(ssid, password);
    Serial.println("Conectando ao Wi-Fi inicial...");
    int tentativas = 0;
    while (WiFi.status() != WL_CONNECTED && tentativas < 15) {
        delay(500);
        Serial.print(".");
        tentativas++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConectado!");
    } else {
        Serial.println("\nFalha ao conectar no setup.");
    }

    dht.begin();
    pinMode(MQ3_PIN, INPUT);

    Serial.println("Comandos Serial:");
    Serial.println("  lote:<nome>");
    Serial.println("  estado_real:<sem risco|madura|alerta|risco de perda>");
}

// =========================================
// Loop principal
// =========================================
void loop() {
    unsigned long cicloInicio = millis();
    // Reconexão
    reconectaWiFi();

    // Leitura Serial para lote e estado_real
    if (Serial.available() > 0) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        if (cmd.startsWith("lote:")) {
            lote = cmd.substring(5);
            lote.trim();
            Serial.print("Lote definido: ");
            Serial.println(lote);

        } else if (cmd.startsWith("estado_real:")) {
            estado_real = cmd.substring(12);
            estado_real.trim();
            Serial.print("Estado real definido: ");
            Serial.println(estado_real);
        }
    }

    // --------- DHT11 ----------
    float temperatura = dht.readTemperature();
    float umidade_ar = dht.readHumidity();

    if (isnan(temperatura) || isnan(umidade_ar)) {
        Serial.println("Erro ao ler DHT11!");
        delay(2000);
        return;
    }

    // --------- MQ-3 (EMA) ----------
    Serial.println("Lendo MQ-3 com EMA (~3.75 minutos)...");
    int mq3_raw = readMQ3_EMA(MQ3_PIN);
    float mq3_tensao = mq3_raw * (3.3 / 4095.0);
    
    // --------- JSON ----------
    String json = "{";
    json += "\"tipoFruta\":\"" + tipoFruta + "\",";
    json += "\"temperatura\":" + String(temperatura, 1) + ",";
    json += "\"umidade_ar\":" + String(umidade_ar, 1) + ",";
    json += "\"mq3_raw\":" + String(mq3_raw) + ",";
    json += "\"mq3_tensao\":" + String(mq3_tensao, 6) + ",";
    json += "\"lote\":\"" + lote + "\"";

    if (estado_real.length() > 0) {
        String sr = estado_real;
        sr.replace("\"", "'");
        json += ",\"estado_real\":\"" + sr + "\"";
    }

    json += "}";

    Serial.println("\nJSON Enviado:");
    Serial.println(json);

    // --------- Envio ----------
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        Serial.print("Tentando conectar a: ");
        Serial.println(serverURL); // IMPRIME A URL ANTES DA REQUISIÇÃO
        
        http.begin(serverURL);
        http.addHeader("Content-Type", "application/json");

        http.addHeader("X-API-KEY", "editKey5");

        int httpCode = http.POST(json);

        Serial.print("HTTP Code: ");
        Serial.println(httpCode); // CÓDIGO DA RESPOSTA

        if (httpCode > 0) {
            // Sucesso (códigos 200, 201, etc.)
            Serial.println("Resposta do Servidor (Sucesso):");
            Serial.println(http.getString());
        } else {
            // Falha (inclui o -1)
            Serial.print("Falha na Requisicao HTTP! Erro: ");
            // NOVO: Exibe a razão do erro para melhor diagnóstico
            Serial.println(http.errorToString(httpCode).c_str()); 
        }
        http.end();

    } else {
        Serial.println("WiFi OFF → Não enviado. Tentará de novo no próximo ciclo.");
    }

    // --------- 5 minutos de ciclo ----------
    unsigned long cicloFim = millis();
    unsigned long tempoPassado = cicloFim - cicloInicio;

    if (tempoPassado < 300000) {
        Serial.print("Aguardando próximo ciclo: ");
        Serial.print((300000 - tempoPassado) / 1000);
        Serial.println(" segundos.");
        delay(300000 - tempoPassado);
    }
}
