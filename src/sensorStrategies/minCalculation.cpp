/**
 * @file MinCalculation.cpp
 * @brief Minimum value calculation strategy.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#include "minCalculation.h"
#include <algorithm>
#include <cmath>
 
float MinCalculation::calculate(float* samples, int count) {
    if (count == 0) return -1;
 
    float minVal = *std::min_element(samples, samples + count);
 
    return round(minVal * 10);
}
 