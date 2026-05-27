/**
 * @file AverageCalculation.cpp
 * @brief Average calculation strategy.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#include "averageCalculation.h"
#include <cmath>
 
float AverageCalculation::calculate(float* samples, int count) {
    if (count == 0) return -1;
 
    float sum = 0;
    for (int i = 0; i < count; i++) sum += samples[i];
 
    float average = sum / count;
 
    return round(average * 10);
}
 