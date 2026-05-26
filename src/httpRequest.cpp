/**
 * @file httpRequest.cpp
 * @brief Handles sending measurement data to the WellAware API.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include <ArduinoJson.h>
#include <Arduino.h>
#include "httpRequest.h"
#include "apiClient.h"

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
 * @param token         JWT token for authentication.
 * @return true if upload was successful (HTTP 200 or 204), false otherwise.
 */
bool sendMessage(Measurement measurements[], int count, float signal, const String& deviceId, const String& token) {
  if (count == 0) {
    Serial.println("No measurements to send.");
    return true;
  }

  JsonDocument doc;
  doc["deviceId"] = deviceId;
  doc["signal"] = signal;

  JsonArray data = doc.createNestedArray("data");
  for (int i = 0; i < count; i++) {
    JsonObject entry = data.createNestedObject();
    entry["level"] = measurements[i].level;
    entry["timestamp"] = measurements[i].timestamp;
  }

  String body;
  serializeJson(doc, body);

  Serial.println("Sending JSON:");
  Serial.println(body);

  ApiClient client;
  ApiResponse res = client.post(String(API_BASE_URL) + "/measurements", body, token);

  Serial.print("HTTP code: ");
  Serial.println(res.code);

  if (res.code == 200 || res.code == 204) {
    Serial.println("Upload successful.");
    return true;
  }

  Serial.println("HTTP error: " + String(res.code));
  return false;
}