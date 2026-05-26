/**
 * @file commands.cpp
 * @brief Handles incoming device commands from the WellAware API.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include <ArduinoJson.h>
#include <Arduino.h>
#include "fetchToken.h"
#include "credentials.h"
#include "setupMode.h"
#include "commands.h"
#include "apiClient.h"
#include "calculationFactory.h"
#include "calculationStore.h"

extern void setCalculationMethod(const String& method);

/**
 * @brief Handles a single command received from the server.
 *
 * @param command    JSON object containing the command type and payload.
 * @param deviceId   The unique device identifier.
 * @param token      JWT token for authentication.
 * @param commandId  The unique command identifier.
 * @return CommandResult indicating success, restart, and setup mode flags.
 */
CommandResult handleCommand(JsonObject command, String deviceId, String token, String commandId) {
  String type = command["type"];

  if (type == "setup_mode") {
    Serial.println("Starting Setup Mode");
    return { true, false, true };
  }

  else if (type == "set_interval") {
    if (command["payload"]["interval"].isNull()) {
      Serial.println("Invalid interval payload");
      return { false, false, false };
    }

    int interval = command["payload"]["interval"].as<int>();
    deviceInterval = interval * 1000;
    Serial.println("Changed sending interval to: " + String(interval));
    saveInterval(deviceInterval);

    return { true, false, false };
  }

  else if (type == "update_wifi") {
    if (
      !command["payload"]["ssid"].is<String>() ||
      !command["payload"]["password"].is<String>()
    ) {
      Serial.println("Invalid WiFi payload");
      return { false, false, false };
    }

    String ssid = command["payload"]["ssid"];
    String password = command["payload"]["password"];
    Serial.println("Updating WiFi credentials");
    saveWifi(ssid, password);

    return { true, true, false };
  }

  else if (type == "restart") {
    Serial.println("Restart command received");
    return { true, true, false };
  }

  else if (type == "change_calculation") {
      if (!command["payload"]["method"].is<String>()) {
          Serial.println("Invalid method payload");
          return { false, false, false };
      }

      String method = command["payload"]["method"].as<String>();

      if (method != "median" && method != "average" &&
          method != "min"    && method != "validation") {
          Serial.println("Unknown method: " + method);
          return { false, false, false };
      }

      saveCalculationMethod(method);
      setCalculationMethod(method);

      Serial.println("Calculation method changed to: " + method);
      return { true, false, false };
  }

  Serial.println("Unknown command type");
  return { false, false, false };
}

/**
 * @brief Fetches and processes all pending commands from the server.
 *
 * Iterates over pending commands, acknowledges each one, and handles
 * setup mode or restart flags after processing.
 *
 * @param deviceId  The unique device identifier.
 * @param token     JWT token for authentication.
 * @return true if the device should enter setup mode, false otherwise.
 */
bool checkForCommands(String deviceId, String token) {
  bool enterSetupMode = false;
  bool shouldRestart = false;

  ApiClient client;
  String url = String(API_BASE_URL) + "/devices/" + deviceId + "/commands";
  ApiResponse res = client.get(url, token);

  if (res.code == 200) {
    Serial.println(res.body);

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, res.body);

    if (error) {
      Serial.println("Failed to parse JSON");
      return false;
    }

    JsonArray commands = doc["commands"];

    if (commands.size() == 0) {
      Serial.println("No pending commands");
      return false;
    }

    for (JsonObject command : commands) {
      String commandId = command["id"];
      String type = command["type"];

      Serial.println("Processing command: " + commandId);

      bool processingAck = acknowledgeCommand(deviceId, token, commandId, "processing");
      if (!processingAck) {
        Serial.println("Failed to acknowledge processing state");
      }

      if (type == "remove_device") {
        acknowledgeCommand(deviceId, token, commandId, "completed");
        removeDeviceFromServer(deviceId, token, commandId);
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

      if (result.enterSetupMode) enterSetupMode = true;
      if (result.requiresRestart) shouldRestart = true;
    }

  } else {
    Serial.println("Failed to fetch commands. HTTP code: " + String(res.code));
  }

  if (shouldRestart) {
    Serial.println("Restarting device...");
    delay(1000);
    ESP.restart();
  }

  return enterSetupMode;
}

/**
 * @brief Sends an acknowledgement for a processed command.
 *
 * @param deviceId   The unique device identifier.
 * @param token      JWT token for authentication.
 * @param commandId  The unique command identifier to acknowledge.
 * @param status     The status to report: "processing", "completed", or "failed".
 * @return true if the server accepted the acknowledgement (HTTP 204), false otherwise.
 */
bool acknowledgeCommand(String deviceId, String token, String commandId, String status) {
  ApiClient client;

  JsonDocument doc;
  doc["status"] = status;
  String body;
  serializeJson(doc, body);

  String url = String(API_BASE_URL) + "/devices/" + deviceId + "/commands/" + commandId + "/ack";
  ApiResponse res = client.patch(url, body, token);

  return res.code == 204;
}

/**
 * @brief Sends a request to remove the device from the server.
 *
 * @param deviceId   The unique device identifier.
 * @param token      JWT token for authentication.
 * @param commandId  The command ID that triggered the removal.
 * @return true if the server confirmed removal (HTTP 204), false otherwise.
 */
bool removeDeviceFromServer(String deviceId, String token, String commandId) {
  ApiClient client;
  String url = String(API_BASE_URL) + "/devices/" + deviceId + "?commandId=" + commandId;
  ApiResponse res = client.del(url, token);
  return res.code == 204;
}