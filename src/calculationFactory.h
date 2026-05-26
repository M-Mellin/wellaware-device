#pragma once
#include "sensorStrategies/ICalculation.h"
#include "sensorStrategies/MedianCalculation.h"
#include "sensorStrategies/AverageCalculation.h"
#include "sensorStrategies/MinCalculation.h"
#include "sensorStrategies/ValidationCalculation.h"
#include <Arduino.h>

inline ICalculation* createCalculation(const String& method) {
    if (method == "average")    return new AverageCalculation();
    if (method == "min")        return new MinCalculation();
    if (method == "validation") return new ValidationCalculation(1.0f, VALIDATION_MIN_INTERVAL_SEC);
    return new MedianCalculation();  // default
}