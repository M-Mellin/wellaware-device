#pragma once
#include "sensorStrategies/ICalculation.h"
#include "sensorStrategies/medianCalculation.h"
#include "sensorStrategies/averageCalculation.h"
#include "sensorStrategies/minCalculation.h"
#include "sensorStrategies/validationCalculation.h"
#include <Arduino.h>
#include "../config.h"

inline ICalculation* createCalculation(const String& method) {
    if (method == "average")    return new AverageCalculation();
    if (method == "min")        return new MinCalculation();
    if (method == "validation") return new ValidationCalculation(1.0f, VALIDATION_MIN_INTERVAL_SEC);
    return new MedianCalculation();  // default
}