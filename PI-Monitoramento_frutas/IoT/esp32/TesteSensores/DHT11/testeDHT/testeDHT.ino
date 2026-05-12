#include "DHT.h"

#define DHTPIN 19       // Pino de dados conforme seu hardware
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println(F("[DIAGNOSTIC] Testing DHT11..."));
  dht.begin();
}

void loop() {
  delay(2000); // DHT11 requer intervalo mínimo de 2s

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("CRITICAL: Failed to read from DHT sensor! Check wiring."));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  |  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C"));
}