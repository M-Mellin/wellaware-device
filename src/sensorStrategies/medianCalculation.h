/**
 * @file MedianCalculation.h
 * @brief Median calculation strategy.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#pragma once
 
#include "ICalculation.h"
 
class MedianCalculation : public ICalculation {
public:
    /**
     * @brief Sorts samples and returns the middle value in millimeters.
     */
    float calculate(float* samples, int count) override;
};
 