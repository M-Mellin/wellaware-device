/**
 * @file ultrasonicSensor.cpp
 * @brief Ultrasonic distance sensor driver with median filtering.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include <Arduino.h>
#include "ultrasonicSensor.h"

/**
 * @brief Takes a single distance measurement from the ultrasonic sensor.
 *
 * Sends a trigger pulse and measures the echo duration to calculate distance.
 *
 * @param ECHO_PIN  GPIO pin connected to the sensor echo output.
 * @param TRIG_PIN  GPIO pin connected to the sensor trigger input.
 * @return Distance in centimeters, or -1 if no echo was received.
 */
float takeSingleMeasurement() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) return -1;

  return duration * 0.0343 / 2;
}

/**
 * @brief Reads the ultrasonic sensor using median filtering over multiple samples.
 *
 * Takes MEDIAN_SAMPLES measurements, discards invalid ones, sorts the valid
 * results, and returns the median value in millimeters.
 *
 * @param ECHO_PIN  GPIO pin connected to the sensor echo output.
 * @param TRIG_PIN  GPIO pin connected to the sensor trigger input.
 * @return Median distance in millimeters, or 0 if no valid samples were received.
 */
float readUltrasonicSensor() {
  float samples[MEDIAN_SAMPLES];
  int validCount = 0;

  for (int i = 0; i < MEDIAN_SAMPLES; i++) {
    float sample = takeSingleMeasurement();

    if (sample != -1) {
      samples[validCount++] = sample;
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