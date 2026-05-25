/**
 * @file fetchToken.cpp
 * @brief Handles device authentication and JWT token retrieval from the WellAware API.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include <ArduinoJson.h>
#include <Arduino.h>
#include "fetchToken.h"
#include "apiClient.h"

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
String fetchToken(String deviceId, String secret) {
  ApiClient client;

  StaticJsonDocument<256> doc;
  doc["deviceId"] = deviceId;
  doc["secret"] = secret;
  String body;
  serializeJson(doc, body);

  ApiResponse res = client.post(String(API_BASE_URL) + "/devices/auth", body);

  Serial.println(res.code);

  if (res.code == 200) {
    Serial.println("Token received");

    StaticJsonDocument<256> resDoc;
    DeserializationError error = deserializeJson(resDoc, res.body);

    if (error) {
      Serial.println("Failed to parse token response");
      return "";
    }

    return resDoc["token"].as<String>();
  }

  Serial.println("fetchToken failed: " + String(res.code));
  return "";
}