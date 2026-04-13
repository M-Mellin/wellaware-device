#include <Arduino.h>
#include "floatSwitch.h"

const int FLOAT_PIN = A1;

void setup() {
  Serial.begin(115200);
  pinMode(FLOAT_PIN, INPUT_PULLUP);

}

void loop() {
  readFloat(digitalRead(FLOAT_PIN));
  delay(2000);
}