#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "fetchToken.h"
#include "credentials.h"
#include "setupMode.h"
#include "commands.h"

bool handleCommand(JsonObject command) {
  String type = command["type"];

  if (type == "setup_mode") {
    Serial.println("Starting Setup Mode");

    return true;
  } else if (type == "set_interval") {
    int interval = command["payload"]["interval"];

    Serial.println("Changed sending interval to: " + String(interval));

    saveInterval(interval);

    return false;
  } else if (type == "update_wifi") {
    String ssid = command["payload"]["ssid"];
    String password = command["payload"]["password"];

    Serial.println("Update WiFi credentials");
    saveWifi(ssid, password);

    ESP.restart();
    return false;
  }

  return false;
}

bool checkForCommands(String deviceId, String token) {
  bool enterSetupMode = false;

  static WiFiClientSecure client;
  client.setInsecure();   

  HTTPClient http;

  http.setTimeout(5000);
  http.setReuse(false);

  http.begin(client, "https://mellin.net/wellaware/api/v1/devices/" + deviceId + "/commands");
  http.addHeader("Authorization", String("Bearer ") + token);

  int code = http.GET();

  if (code == 200) {
    String response = http.getString();
    Serial.println(response);

    DynamicJsonDocument doc(2048);

    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.println("Failed to parse JSON");
      http.end();
      return false;
    }

    JsonArray commands = doc["commands"];

    if (commands.size() == 0) {
      Serial.println("No pending commands");
      http.end();
      return false;
    }

    for (JsonObject command : commands) {
      if (handleCommand(command)) {
        enterSetupMode = true;
      }
    } 

  } else {
    Serial.println("Failed to fetch commands");
  }
  
  http.end();

  return enterSetupMode;
}