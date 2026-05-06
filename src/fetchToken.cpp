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

  http.begin(client, "https://mellin.net/wellaware/api/v1/device/auth");

  http.addHeader("Content-Type", "application/json");

  String json = "{";
  json += "\"deviceId\":\"" + deviceId + "\",";
  json += "\"secret\":\"" + secret + "\"";
  json += "}";

  int code = http.POST(json);

  String response = http.getString();

  Serial.println(code);

  http.end();

  if (code == 200) {
    Serial.println("Token received");
    DynamicJsonDocument doc(2048);

    deserializeJson(doc, response);

    String token = doc["token"].as<String>();

    return token;
  } else {
    Serial.println(http.errorToString(code));
    return "";
  }
}