/**
 * @file ValidationCalculation.cpp
 * @brief Logical validation strategy — rejects readings that deviate too much
 *        from the previous value within a short time window.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include "validationCalculation.h"
#include <Arduino.h>
#include <algorithm>
#include <cmath>
#include <ctime>
 
ValidationCalculation::ValidationCalculation(float maxChangePct, unsigned long minIntervalSec)
    : _maxChangePct(maxChangePct),
      _minIntervalSec(minIntervalSec),
      _lastValue(-1),
      _lastTimestamp(0) {}
 
float ValidationCalculation::calculate(float* samples, int count) {
    if (count == 0) return -1;
 
    // Use median as candidate
    std::sort(samples, samples + count);
    float candidate = round(samples[count / 2] * 10);
 
    if (_lastValue < 0) {
        _lastValue = candidate;
        _lastTimestamp = (unsigned long)time(nullptr);
        return candidate;
    }
 
    unsigned long now     = (unsigned long)time(nullptr);
    unsigned long elapsed = now - _lastTimestamp;
 
    if (elapsed < _minIntervalSec) {
        float changePct = fabsf(candidate - _lastValue) / _lastValue * 100.0f;
 
        if (changePct > _maxChangePct) {
            Serial.printf("[Validation] Rejected: %.0f mm (%.1f%% change in %lus)\n", candidate, changePct, elapsed);
            return -1;
        }
    }
 
    _lastValue = candidate;
    _lastTimestamp = now;
 
    return candidate;
}
 