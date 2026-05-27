/**
 * @file SensorProcessor.h
 * @brief Connects an ISensor with an ICalculation strategy.
 *
 * Collects raw samples from the sensor (with retry on no-signal),
 * then delegates the calculation to the chosen strategy.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#pragma once
 
#include "ISensor.h"
#include "ICalculation.h"
 
class SensorProcessor {
public:
    /**
     * @param sensor      Hardware sensor to read from.
     * @param calculation Calculation strategy to apply.
     * @param samples     Number of raw samples to collect per reading.
     * @param maxRetries  Number of retries per sample on no-signal.
     */
    SensorProcessor(ISensor* sensor, ICalculation* calculation, int samples = 12, int maxRetries = 3);
 
    /**
     * @brief Collects samples and applies the calculation strategy.
     * @return Distance in millimeters, or -1 if no valid result.
     */
    float read();
 
private:
    ISensor*      _sensor;
    ICalculation* _calculation;
    int           _samples;
    int           _maxRetries;
};