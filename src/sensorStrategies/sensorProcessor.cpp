/**
 * @file SensorProcessor.cpp
 * @brief Connects an ISensor with an ICalculation strategy.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#include "SensorProcessor.h"
#include <Arduino.h>
 
SensorProcessor::SensorProcessor(ISensor* sensor, ICalculation* calculation, int samples, int maxRetries)
    : _sensor(sensor),
      _calculation(calculation),
      _samples(samples),
      _maxRetries(maxRetries) {}
 
float SensorProcessor::read() {
    float buffer[_samples];
    int validCount = 0;
 
    for (int i = 0; i < _samples; i++) {
        float raw = -1;
 
        for (int attempt = 0; attempt <= _maxRetries; attempt++) {
            raw = _sensor->measure();
 
            if (raw >= 0) break;
 
            if (attempt < _maxRetries) {
                Serial.printf("[Sensor] No signal, retry %d/%d...\n", attempt + 1, _maxRetries);
                delay(500);
            }
        }
 
        if (raw >= 0) {
            buffer[validCount++] = raw;
        }
 
        delayMicroseconds(30000);
    }
 
    if (validCount == 0) {
        Serial.println("[Sensor] No valid samples.");
        return -1;
    }
 
    return _calculation->calculate(buffer, validCount);
}