/**
 * @file ultrasonicSensor.h
 * @brief Ultrasonic distance sensor driver with median filtering.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include "../config.h"

/**
 * @brief Takes a single distance measurement from the ultrasonic sensor.
 *
 * @param ECHO_PIN  GPIO pin connected to the sensor echo output.
 * @param TRIG_PIN  GPIO pin connected to the sensor trigger input.
 * @return Distance in centimeters, or -1 if no echo was received.
 */
float takeSingleMeasurement();

/**
 * @brief Reads the ultrasonic sensor using median filtering over multiple samples.
 *
 * @param ECHO_PIN  GPIO pin connected to the sensor echo output.
 * @param TRIG_PIN  GPIO pin connected to the sensor trigger input.
 * @return Median distance in millimeters, or 0 if no valid samples were received.
 */
float readUltrasonicSensor();

#endif