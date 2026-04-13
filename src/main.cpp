#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Startar...");
}

void loop() {
  Serial.println("Tick");
  delay(2000);
}