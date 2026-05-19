#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "fetchToken.h"
#include "credentials.h"
#include "setupMode.h"
#include "commands.h"

CommandResult handleCommand(JsonObject command, String deviceId, String token, String commandId) {
  String type = command["type"];

  if (type == "setup_mode") {
    Serial.println("Starting Setup Mode");

    return {
      true,
      false,
      true
    };
  }

  else if (type == "set_interval") {

    if (command["payload"]["interval"].isNull()) {
      Serial.println("Invalid interval payload");

      return {
        false,
        false,
        false
      };
    }

    int interval =
      command["payload"]["interval"].as<int>();

    deviceInterval = interval * 1000;

    Serial.println(
      "Changed sending interval to: " +
      String(interval)
    );

    saveInterval(deviceInterval);

    return {
      true,
      false,
      false
    };
  }

  else if (type == "update_wifi") {

    if (
      !command["payload"]["ssid"].is<String>() ||
      !command["payload"]["password"].is<String>()
    ) {
      Serial.println("Invalid WiFi payload");

      return {
        false,
        false,
        false
      };
    }

    String ssid = command["payload"]["ssid"];
    String password = command["payload"]["password"];

    Serial.println("Updating WiFi credentials");

    saveWifi(ssid, password);

    return {
      true,
      true,
      false
    };
  }

  else if (type == "restart") {

    Serial.println("Restart command received");

    return {
      true,
      true,
      false
    };
  }

  Serial.println("Unknown command type");

  return {
    false,
    false,
    false
  };
}

bool checkForCommands(String deviceId, String token) {
  bool enterSetupMode = false;
  bool shouldRestart = false;

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

    DynamicJsonDocument doc(4096);

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
      String commandId = command["id"];
      String type = command["type"];

      Serial.println(
        "Processing command: " + commandId
      );

      bool processingAck = 
        acknowledgeCommand(
          deviceId,
          token,
          commandId,
          "processing"
        );

      if (!processingAck) {
        Serial.println(
          "Failed to acknowledge processing state"
        );
      }

      if (type == "remove_device") {
        acknowledgeCommand(deviceId, token, commandId, "completed");
        removeDeviceFromServer(deviceId, token, commandId);
        
        http.end();
        clearAllData();
        startSetupMode();
        return false;
      }

      CommandResult result = handleCommand(command, deviceId, token, commandId);

      acknowledgeCommand(
        deviceId,
        token,
        commandId,
        result.success ? "completed" : "failed"
      );

      if (result.enterSetupMode) {
        enterSetupMode = true;
      }

      if (result.requiresRestart) {
        shouldRestart = true;
      }
    }

  } else {
    Serial.println(
      "Failed to fetch commands. HTTP code: " +
      String(code)
    );
  }
  
  http.end();

  if (shouldRestart) {

    Serial.println("Restarting device...");

    delay(1000);

    ESP.restart();
  }

  return enterSetupMode;
}

bool acknowledgeCommand(
  String deviceId,
  String token,
  String commandId,
  String status
) {

  static WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;

  http.setTimeout(5000);
  http.setReuse(false);

  String url =
    "https://mellin.net/wellaware/api/v1/devices/" +
    deviceId +
    "/commands/" +
    commandId +
    "/ack";

  http.begin(client, url);

  http.addHeader("Authorization", "Bearer " + token);
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(256);

  doc["status"] = status;

  String body;
  serializeJson(doc, body);

  int code = http.PATCH(body);

  http.end();

  return code == 204;
}

bool removeDeviceFromServer(String deviceId, String token, String commandId) {
  static WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.setTimeout(5000);
  http.setReuse(false);

  String url =
    "https://mellin.net/wellaware/api/v1/devices/" +
    deviceId +
    "?commandId=" +
    commandId;

  http.begin(client, url);
  http.addHeader("Authorization", "Bearer " + token);
  http.addHeader("Content-Type", "application/json");

  int code = http.sendRequest("DELETE");
  http.end();

  return code == 204;
}