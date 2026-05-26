/**
 * @file ISensor.h
 * @brief Abstract hardware sensor interface (Strategy Pattern).
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#pragma once
 
class ISensor {
public:
    virtual ~ISensor() = default;
 
    /**
     * @brief Takes a single raw distance measurement.
     * @return Distance in centimeters, or -1 if no valid reading.
     */
    virtual float measure() = 0;
};
 