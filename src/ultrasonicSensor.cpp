#include <Arduino.h>
#include "ultrasonicSensor.h"

#define MEDIAN_SAMPLES 12

float takeSingleMeasurement(int ECHO_PIN, int TRIG_PIN) {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) return -1;

  return duration * 0.0343 / 2;
}

float readUltrasonicSensor(int ECHO_PIN, int TRIG_PIN) {
  float samples[MEDIAN_SAMPLES];
  int validCount = 0;

  for (int i = 0; i < MEDIAN_SAMPLES; i++) {
    float sample = takeSingleMeasurement(ECHO_PIN, TRIG_PIN);

    if (sample != -1) {
      samples[validCount ++] = sample;
    }

    delayMicroseconds(30000);
  }

  if (validCount == 0) {
    Serial.println("No signal");
    return 0;
  }

  for (int i = 0; i < validCount - 1; i++) {
    for (int j = 0; j < validCount - i - 1; j++) {
      if (samples[j] > samples[j + 1]) {
        float temp = samples[j];
        samples[j] = samples[j + 1];
        samples[j + 1] = temp;
      }
    }
  }
  
  for (int i = 0; i < validCount; i++) {
    Serial.println(samples[i]);
  }

  float median = samples[validCount / 2];

  Serial.print("Distance: ");
  Serial.print(median * 10);
  Serial.println(" mm");

  return round(median * 10);
}