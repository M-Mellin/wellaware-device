#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>

void sendMessage(float distance, float signal, const String& floatState, const String& deviceId) {

  static WiFiClientSecure client;
  client.setInsecure();   

  HTTPClient http;

  http.setTimeout(5000);
  http.setReuse(false);

  http.begin(client, "https://mellin.net/wellaware/api/v1/measurements");

  http.addHeader("Content-Type", "application/json");

  String json = "{\"level\":" + String(distance) +
                ",\"floatState\":\"" + floatState +
                "\",\"signal\":" + String(signal) +
                ",\"deviceId\":\"" + deviceId + "\"}";

  Serial.println("Sending JSON:");
  Serial.println(json);

  int code = http.POST(json);

  Serial.print("HTTP code: ");
  Serial.println(code);

  if (code > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("HTTP error: ");
    Serial.println(http.errorToString(code));
  }

  http.end();

  return;
}