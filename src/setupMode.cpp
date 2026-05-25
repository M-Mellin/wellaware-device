#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include "credentials.h"
#include "fetchToken.h"
#include "apiClient.h"

WebServer server(80);

bool setDeviceProvisioning(String deviceId, String token) {
  ApiClient client;
  ApiResponse res = client.patch(String(API_BASE_URL) + "/devices/" + deviceId, "{}", token);

  Serial.print("HTTP code: ");
  Serial.println(res.code);

  if (res.code == 204) {
    Serial.println("Provisioning successful.");
    return true;
  }

  Serial.println("HTTP error: " + String(res.code));
  return false;
}

void handleSetup() {
  Serial.println("---- INCOMING REQUEST ----");
  Serial.println(server.method());
  Serial.println(server.uri());

  String body = server.arg("plain");

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
  const char* secret   = doc["secret"];
  const char* ssid     = doc["ssid"];
  const char* password = doc["password"];
  int interval         = doc["interval"] | 0;

  if (!deviceId || !secret || !ssid || !password) {
    server.send(400, "application/json", "{\"error\":\"Missing fields\"}");
    return;
  }

  if (interval < 5 || interval > 3600) {
    server.send(400, "application/json", "{\"error\":\"interval out of range (5-3600)\"}");
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
  WiFi.softAP(SETUP_AP_SSID);

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
    if (millis() - start > SETUP_TIMEOUT_MS) {
      Serial.println("Setup timeout - restarting");
      ESP.restart();
    }
  }
}