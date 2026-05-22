#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Arduino.h>

String fetchToken(String deviceId, String secret) {
  static WiFiClientSecure client;

  client.setInsecure();

  HTTPClient http;

  http.setTimeout(5000);
  http.setReuse(false);

  http.begin(client, "https://mellin.net/wellaware/api/v1/devices/auth");

  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<256> doc;

  doc["deviceId"] = deviceId;
  doc["secret"] = secret;

  String json;
  serializeJson(doc, json);

  int code = http.POST(json);

  String response = http.getString();

  Serial.println(code);

  http.end();

  if (code == 200) {
    Serial.println("Token received");
    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.println("Failed to parse token response");
      return "";
    }

    String token = doc["token"].as<String>();

    return token;
  } else {
    Serial.println(http.errorToString(code));
    Serial.println(response);
    return "";
  }
}