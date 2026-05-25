#ifndef COMMANDS_H
#define COMMANDS_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../config.h"

struct CommandResult {
  bool success;
  bool requiresRestart;
  bool enterSetupMode;
};

extern int deviceInterval;
CommandResult handleCommand(JsonObject command, String deviceId, String token, String commandId);
bool checkForCommands(String deviceId, String token);
bool acknowledgeCommand(String deviceId, String token, String commandId, String status);
bool removeDeviceFromServer(String deviceId, String token, String commandId);

#endif