#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include "httpRequest.h"

bool sendMessage(Measurement measurements[], int count, float signal, const String& floatState, const String& deviceId) {

  if (count == 0) {
    Serial.println("No measurments to send.");
    return true;
  }

  static WiFiClientSecure client;
  client.setInsecure();   

  HTTPClient http;

  http.setTimeout(5000);
  http.setReuse(false);

  http.begin(client, "https://mellin.net/wellaware/api/v1/measurements");

  http.addHeader("Content-Type", "application/json");

  String json = "{";
  json += "\"deviceId\":\"" + deviceId + "\",";
  json += "\"signal\":" + String(signal) + ",";
  json += "\"floatState\":\"" + floatState + "\",";
  json += "\"data\":[";

  for (int i = 0; i < count; i++) {
    json += "{";
    json += "\"level\":" + String(measurements[i].level) + ",";
    json += "\"timestamp\":\"" + String(measurements[i].timestamp) + "\"";
    json += "}";

    if (i < count - 1) {
      json += ",";
    }
  }

  json += "]";
  json += "}";

  Serial.println("Sending JSON:");
  Serial.println(json);

  int code = http.POST(json);

  Serial.print("HTTP code: ");
  Serial.println(code);

  bool success = false;

  if (code == 204 || code == 200) {
    success = true;
    Serial.println("Upload successful.");
  } else {
    Serial.print("HTTP error: ");
    Serial.println(http.errorToString(code));
  }

  http.end();

  return success;
}