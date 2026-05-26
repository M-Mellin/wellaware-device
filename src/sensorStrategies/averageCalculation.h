/**
 * @file AverageCalculation.h
 * @brief Average calculation strategy.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#pragma once
 
#include "ICalculation.h"
 
class AverageCalculation : public ICalculation {
public:
    /**
     * @brief Returns the mean of all samples in millimeters.
     */
    float calculate(float* samples, int count) override;
};
 