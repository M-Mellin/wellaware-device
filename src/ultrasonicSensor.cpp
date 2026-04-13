#include <Arduino.h>
#include "ultrasonicSensor.h"

void readUltrasonicSensor(int ECHO_PIN, int TRIG_PIN) {
  long duration;
  float distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    Serial.println("No signal");
  } else {
    distance = duration * 0.0343 / 2;
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}