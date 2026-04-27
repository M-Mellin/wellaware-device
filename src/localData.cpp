#include <Arduino.h>
#include "localData.h"


void addMeasurement(Measurement pendingMeasurements[], float level, unsigned long timestamp, int& measurementCount, int MAX_MEASUREMENTS) {
  if (measurementCount >= MAX_MEASUREMENTS) {
    Serial.println("Buffer full. Measurment skipped.");
    return;
  }

  pendingMeasurements[measurementCount].level = level;
  pendingMeasurements[measurementCount].timestamp = timestamp;

  measurementCount++;

  Serial.print("Saved locally. Count: ");
  Serial.println(measurementCount);
}

void clearMeasurements(int& measurmentCount) {
  measurmentCount = 0;
  Serial.println("Measurement cleared.");
}