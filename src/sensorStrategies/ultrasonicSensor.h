/**
 * @file ultrasonicSensor.h
 * @brief Ultrasonic distance sensor hardware driver.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#pragma once
 
#include <Arduino.h>
#include "ISensor.h"
 
class UltrasonicSensor : public ISensor {
public:
    /**
     * @param trigPin  GPIO pin connected to the sensor trigger input.
     * @param echoPin  GPIO pin connected to the sensor echo output.
     */
    UltrasonicSensor(int trigPin, int echoPin);
 
    /**
     * @brief Takes a single raw distance measurement.
     * @return Distance in centimeters, or -1 if no echo was received.
     */
    float measure() override;
 
private:
    int _trigPin;
    int _echoPin;
};
 