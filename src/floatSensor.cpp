#include <Arduino.h>
#include "floatSensor.h"

bool readFloatSensor(int PIN) {
  int value = digitalRead(PIN);
  bool isClosed = (value == LOW);

  Serial.println(isClosed ? "CLOSED" : "OPEN");
  return isClosed;
}