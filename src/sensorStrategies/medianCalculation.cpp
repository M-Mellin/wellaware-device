/**
 * @file MedianCalculation.cpp
 * @brief Median calculation strategy.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#include "MedianCalculation.h"
#include <algorithm>
#include <cmath>
 
float MedianCalculation::calculate(float* samples, int count) {
    if (count == 0) return -1;
 
    std::sort(samples, samples + count);
 
    float median = samples[count / 2];
 
    return round(median * 10);
}
