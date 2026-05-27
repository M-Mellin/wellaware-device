/**
 * @file setupMode.h
 * @brief Handles device setup mode, provisioning, and the local HTTP configuration server.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#ifndef SETUP_MODE_H
#define SETUP_MODE_H

#include <Arduino.h>
#include "../config.h"

/**
 * @brief Handles incoming POST requests to the /setup endpoint.
 *
 * Expects a JSON body with deviceId, secret, ssid, password, and interval.
 * Saves credentials and restarts the device on success.
 */
void handleSetup();

/**
 * @brief Starts the device in setup mode as a WiFi access point.
 *
 * Creates a local HTTP server on port 80 with a /setup endpoint.
 * Automatically restarts after SETUP_TIMEOUT_MS if no setup is performed.
 */
void startSetupMode();

/**
 * @brief Sends a provisioning request to mark the device as registered on the server.
 *
 * @param deviceId  The unique device identifier.
 * @param token     JWT token for authentication.
 * @return true if provisioning was successful (HTTP 204), false otherwise.
 */
bool setDeviceProvisioning(String deviceId, String token);

#endif