#include <ArduinoJson.h>
#include <Arduino.h>
#include "httpRequest.h"
#include "apiClient.h"

bool sendMessage(Measurement measurements[], int count, float signal, const String& deviceId, const String& token) {
  if (count == 0) {
    Serial.println("No measurements to send.");
    return true;
  }

  DynamicJsonDocument doc(4096);
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