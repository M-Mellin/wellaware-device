/**
 * @file UltrasonicSensor.cpp
 * @brief Ultrasonic distance sensor hardware driver.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#include "ultrasonicSensor.h"
 
UltrasonicSensor::UltrasonicSensor(int trigPin, int echoPin)
    : _trigPin(trigPin), _echoPin(echoPin) {
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);
}
 
float UltrasonicSensor::measure() {
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);
 
    long duration = pulseIn(_echoPin, HIGH, 30000);
 
    if (duration == 0) return -1;
 
    return duration * 0.0343f / 2.0f;
}
 