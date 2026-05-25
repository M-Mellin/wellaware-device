#include <ArduinoJson.h>
#include <Arduino.h>
#include "fetchToken.h"
#include "apiClient.h"

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