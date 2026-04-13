#include <Arduino.h>
#include "floatSwitch.h"

void readFloat(int value) {
  Serial.println(value == LOW ? "CLOSED" : "OPEN");
}