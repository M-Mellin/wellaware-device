#ifndef COMMANDS_H
#define COMMANDS_H
#include <Arduino.h>
#include <ArduinoJson.h>

bool handleCommand(JsonObject command);
bool checkForCommands(String deviceId, String token);


#endif