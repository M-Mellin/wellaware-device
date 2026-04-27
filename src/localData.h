#ifndef LOCAL_DATA_H
#define LOCAL_DATA_H
#include "httpRequest.h"

void addMeasurement(Measurement pendingMeasurements[], float level, unsigned long timestamp, int& measurementCount, int MAX_MEASUREMENTS);
void clearMeasurements(int& measurementCount);

#endif