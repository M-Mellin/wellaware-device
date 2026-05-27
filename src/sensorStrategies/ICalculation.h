/**
 * @file ICalculation.h
 * @brief Abstract calculation strategy interface (Strategy Pattern).
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#pragma once
 
class ICalculation {
public:
    virtual ~ICalculation() = default;
 
    /**
     * @brief Processes raw samples into a single distance value.
     * @param samples  Array of raw readings in centimeters.
     * @param count    Number of valid samples.
     * @return Processed distance in millimeters, or -1 if invalid.
     */
    virtual float calculate(float* samples, int count) = 0;
};
 