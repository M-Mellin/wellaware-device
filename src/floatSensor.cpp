#include <Arduino.h>
#include "floatSensor.h"

String readFloatSensor(int PIN) {
  int value = digitalRead(PIN);
  Serial.println(value == LOW ? "CLOSED" : "OPEN");
  return value == LOW ? "CLOSED" : "OPEN";
}