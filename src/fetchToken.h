/**
 * @file fetchToken.h
 * @brief Handles device authentication and JWT token retrieval from the WellAware API.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#pragma once

#include <Arduino.h>
#include "../config.h"

/**
 * @brief Authenticates the device and retrieves a JWT token from the server.
 *
 * Sends the device ID and secret to the auth endpoint. On success,
 * parses and returns the JWT token from the response.
 *
 * @param deviceId  The unique device identifier.
 * @param secret    The device secret used for authentication.
 * @return String containing the JWT token, or empty string on failure.
 */
String fetchToken(String deviceId, String secret);