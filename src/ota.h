/**
 * @file ota.h
 * @brief Over-the-air firmware update management for the WellAware device.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#ifndef OTA_H
#define OTA_H

#include <Arduino.h>
#include "../config.h"

/**
 * @brief Checks the server for a newer firmware version and performs OTA if available.
 *
 * Compares the latest version from the server against the locally saved version.
 * Downloads and installs the update if a newer version is found, then restarts.
 *
 * @param deviceId  The unique device identifier.
 * @param jwtToken  JWT token for authentication.
 */
void checkForOtaUpdate(const String& deviceId, const String& jwtToken);

#endif