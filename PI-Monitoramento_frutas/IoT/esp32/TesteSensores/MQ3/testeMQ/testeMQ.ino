#define MQ3PIN 34 // ADC1_CH6 (Seguro para Wi-Fi no ESP32)

void setup() {
  Serial.begin(115200);
  Serial.println(F("[DIAGNOSTIC] Testing MQ-3 Gas Sensor..."));
  
  // Resolução de 12 bits padrão do ESP32 (0 a 4095)
  analogReadResolution(12);
}

void loop() {
  int rawValue = analogRead(MQ3PIN);
  
  // Cálculo simples de voltagem para referência técnica
  float voltage = (rawValue / 4095.0) * 3.3;

  Serial.print(F("MQ-3 Raw Value: "));
  Serial.print(rawValue);
  Serial.print(F(" | Voltage: "));
  Serial.print(voltage);
  Serial.println(F("V"));

  // Lógica de alerta baseada nos seus thresholds de machinelearning.py
  if (rawValue > 3000) {
    Serial.println(F(">> ALERT: High Gas Concentration Detected!"));
  }

  delay(500); // Leitura mais frequente para observar picos
}