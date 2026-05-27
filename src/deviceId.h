/**
 * @file deviceId.h
 * @brief Generates a unique device identifier based on the ESP32 chip MAC address.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#ifndef DEVICE_ID_H
#define DEVICE_ID_H

#include <Arduino.h>

/**
 * @brief Generates a unique device ID derived from the ESP32 eFuse MAC address.
 *
 * Format: "wellaware-XXXXYYYYYYYY" where X is the upper 16 bits
 * and Y is the lower 32 bits of the chip MAC address.
 *
 * @return String containing the unique device identifier.
 */
String getDeviceId();

#endif