#include <Arduino.h>
#include "floatSensor.h"

void readFloatSensor(int PIN) {
  int value = digitalRead(PIN);
  Serial.println(value == LOW ? "CLOSED" : "OPEN");
}