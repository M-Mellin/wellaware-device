#pragma once
#include <Arduino.h>
#include "../config.h"

struct Measurement {
  float level;
  unsigned long timestamp;
};

void addMeasurement(Measurement pendingMeasurements[], float level, unsigned long timestamp, int& measurementCount, int MAX_MEASUREMENTS);
void clearMeasurements(int& measurementCount);
bool saveMeasurementsToFlash(Measurement pendingMeasurements[], int measurementCount);
bool loadMeasurementsFromFlash(Measurement pendingMeasurements[], int& measurementCount, int MAX_MEASUREMENTS);