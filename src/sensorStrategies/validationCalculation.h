/**
 * @file ValidationCalculation.h
 * @brief Logical validation strategy — rejects readings that deviate too much
 *        from the previous value within a short time window.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#pragma once
 
#include "ICalculation.h"
 
class ValidationCalculation : public ICalculation {
public:
    /**
     * @param maxChangePct    Max allowed % change from last reading within minIntervalSec.
     * @param minIntervalSec  Time window in seconds where the % rule applies.
     */
    ValidationCalculation(float maxChangePct = 1.0f, unsigned long minIntervalSec = 60);
 
    /**
     * @brief Calculates the median of samples, then validates against last known value.
     * @return Distance in millimeters, or -1 if rejected.
     */
    float calculate(float* samples, int count) override;
 
private:
    float         _maxChangePct;
    unsigned long _minIntervalSec;
    float         _lastValue;
    unsigned long _lastTimestamp;
};
 