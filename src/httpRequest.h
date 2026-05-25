/**
 * @file httpRequest.h
 * @brief Handles sending measurement data to the WellAware API.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <Arduino.h>
#include "localData.h"
#include "../config.h"

/**
 * @brief Sends a batch of measurements to the server.
 *
 * Serializes the measurements into a JSON payload and posts them
 * to the measurements endpoint. Returns true immediately if count is 0.
 *
 * @param measurements  Array of Measurement structs to send.
 * @param count         Number of measurements in the array.
 * @param signal        Current WiFi signal strength in dBm.
 * @param deviceId      The unique device identifier.
 * @param secret        JWT token for authentication.
 * @return true if upload was successful (HTTP 200 or 204), false otherwise.
 */
bool sendMessage(
  Measurement measurements[],
  int count,
  float signal,
  const String& deviceId,
  const String& secret
);

#endif