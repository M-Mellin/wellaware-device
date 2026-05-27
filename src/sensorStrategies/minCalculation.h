/**
 * @file MinCalculation.h
 * @brief Minimum value calculation strategy.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#pragma once
 
#include "ICalculation.h"
 
class MinCalculation : public ICalculation {
public:
    /**
     * @brief Returns the smallest sample value in millimeters.
     */
    float calculate(float* samples, int count) override;
};