/**
 * @file deviceId.cpp
 * @brief Generates a unique device identifier based on the ESP32 chip MAC address.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include <Arduino.h>
#include "deviceId.h"

/**
 * @brief Generates a unique device ID derived from the ESP32 eFuse MAC address.
 *
 * Format: "wellaware-XXXXYYYYYYYY" where X is the upper 16 bits
 * and Y is the lower 32 bits of the chip MAC address.
 *
 * @return String containing the unique device identifier.
 */
String getDeviceId() {
  uint64_t chipId = ESP.getEfuseMac();

  char deviceId[32];
  snprintf(
    deviceId,
    sizeof(deviceId),
    "wellaware-%04X%08X",
    (uint16_t)(chipId >> 32),
    (uint32_t)chipId
  );

  return String(deviceId);
}