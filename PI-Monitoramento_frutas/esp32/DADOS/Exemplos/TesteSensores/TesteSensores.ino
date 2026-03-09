#include "DHT.h"

// Definições de Hardware
#define DHTPIN 15       
#define DHTTYPE DHT11
#define MQ3PIN 34      // ADC1_CH6 - Seguro para uso com Wi-Fi


DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Serial a 115200 é o padrão para evitar lixo no monitor do ESP32
  Serial.begin(115200); 
  delay(1000); 
  
  Serial.println(F("\n===================================="));
  Serial.println(F("     SENSOR DIAGNOSTIC   "));
  Serial.println(F("===================================="));
  
  dht.begin();
  
  // No ESP32, o ADC tem resolução de 12 bits (0-4095) por padrão
  analogReadResolution(12);
}

void loop() {
  // O DHT11 é lento, 2 segundos é o intervalo ideal
  delay(2000);

  // Leitura DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Leitura MQ3
  int mq3Value = analogRead(MQ3PIN);

  // Saída formatada para o Serial Monitor
  Serial.print(F("[SYS] "));

  if (isnan(h) || isnan(t)) {
    Serial.print(F("DHT11: ERROR | "));
  } else {
    Serial.print(F("Hello World - DHT11 [T: "));
    Serial.print(t);
    Serial.print(F("C, H: "));
    Serial.print(h);
    Serial.print(F("%] | "));
  }

  Serial.print(F("Hello World - MQ3 [Raw: "));
  Serial.print(mq3Value);
  Serial.println(F("]"));
  
  // Dica técnica: se o MQ3 estiver abaixo de 100, ele ainda está esfriando.
  if(mq3Value < 100) {
    Serial.println(F(" >> MQ3 em aquecimento... aguarde 60s."));
  }
}