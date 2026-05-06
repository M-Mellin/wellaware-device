#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "credentials.h"

WebServer server(80);

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

  if (!deviceId || !secret || !ssid || !password) {
    server.send(400, "application/json", "{\"error\":\"Missing fields\"}");
    return;
  }

  saveCredentials(String(deviceId), String(secret));
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