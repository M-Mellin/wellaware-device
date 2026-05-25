/**
 * @file commands.h
 * @brief Command handling for incoming device commands from the WellAware API.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#ifndef COMMANDS_H
#define COMMANDS_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../config.h"

/**
 * @brief Result of a processed command.
 */
struct CommandResult {
  bool success;
  bool requiresRestart;
  bool enterSetupMode;
};

extern int deviceInterval; ///< Current measurement interval in milliseconds

/**
 * @brief Handles a single command received from the server.
 *
 * @param command    JSON object containing the command type and payload.
 * @param deviceId   The unique device identifier.
 * @param token      JWT token for authentication.
 * @param commandId  The unique command identifier.
 * @return CommandResult indicating success, restart, and setup mode flags.
 */
CommandResult handleCommand(JsonObject command, String deviceId, String token, String commandId);

/**
 * @brief Fetches and processes all pending commands from the server.
 *
 * @param deviceId  The unique device identifier.
 * @param token     JWT token for authentication.
 * @return true if the device should enter setup mode, false otherwise.
 */
bool checkForCommands(String deviceId, String token);

/**
 * @brief Sends an acknowledgement for a processed command.
 *
 * @param deviceId   The unique device identifier.
 * @param token      JWT token for authentication.
 * @param commandId  The unique command identifier to acknowledge.
 * @param status     The status to report: "processing", "completed", or "failed".
 * @return true if the server accepted the acknowledgement (HTTP 204), false otherwise.
 */
bool acknowledgeCommand(String deviceId, String token, String commandId, String status);

/**
 * @brief Sends a request to remove the device from the server.
 *
 * @param deviceId   The unique device identifier.
 * @param token      JWT token for authentication.
 * @param commandId  The command ID that triggered the removal.
 * @return true if the server confirmed removal (HTTP 204), false otherwise.
 */
bool removeDeviceFromServer(String deviceId, String token, String commandId);

#endif