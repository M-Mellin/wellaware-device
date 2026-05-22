#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "credentials.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "fetchToken.h"

WebServer server(80);

bool setDeviceProvisioning(String deviceId, String token) {
  static WiFiClientSecure client;
  client.setInsecure();   

  HTTPClient http;

  http.setTimeout(5000);
  http.setReuse(false);

  http.begin(client, "https://mellin.net/wellaware/api/v1/devices/" + deviceId);

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + token);

  int code = http.PATCH("{}");

  Serial.print("HTTP code: ");
  Serial.println(code);

  bool success = false;

  if (code == 204) {
    success = true;
    Serial.println("Upload successful.");
  } else {
    Serial.print("HTTP error: ");
    Serial.println(http.errorToString(code));
  }

  http.end();

  return success;
}

void handleSetup() {
  Serial.println("SETUP ENDPOINT HIT");

  Serial.println("---- INCOMING REQUEST ----");
  Serial.println(server.method());
  Serial.println(server.uri());

  String body = server.arg("plain");
  Serial.println("Body:");
  Serial.println(body);

  if (body.length() == 0) {
    server.send(400, "application/json", "{\"error\":\"Empty body\"}");
    return;
  }

  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  const char* deviceId = doc["deviceId"];
  const char* secret = doc["secret"];
  const char* ssid = doc["ssid"];
  const char* password = doc["password"];
  const int interval = doc["interval"];

  if (!deviceId || !secret || !ssid || !password) {
    server.send(400, "application/json", "{\"error\":\"Missing fields\"}");
    return;
  }

  saveCredentials(String(deviceId), String(secret), false);

  saveInterval(interval * 1000);

  saveWifi(String(ssid), String(password));

  server.send(200, "application/json", "{\"status\":\"ok\"}");

  delay(1000);
  ESP.restart();
}

void startSetupMode() {
  WiFi.softAP("Device-Setup");

  Serial.println("Setup mode started");
  Serial.println(WiFi.softAPIP());

  server.on("/setup", HTTP_POST, handleSetup);

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/plain", "Device is in setup mode");
  });

  server.onNotFound([]() {
    Serial.print("Not found: ");
    Serial.println(server.uri());
    server.send(404, "application/json", "{\"error\":\"Not found\"}");
  });

  server.begin();

  unsigned long start = millis();

  while (true) {
    server.handleClient();

    if (millis() - start > 300000) {
      ESP.restart();
    }
  }
}
