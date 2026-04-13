#include <Arduino.h>
#include "floatSensor.h"
#include "ultrasonicSensor.h"

const int FLOAT_PIN = A1;
const int ECHO_PIN = 4;
const int TRIG_PIN = 5;

void setup() {
  Serial.begin(115200);
  pinMode(FLOAT_PIN, INPUT_PULLUP); // FLOAT SWITCH
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  readFloatSensor(FLOAT_PIN);
  delay(1000);
  readUltrasonicSensor(ECHO_PIN, TRIG_PIN);
  delay(1000);
}