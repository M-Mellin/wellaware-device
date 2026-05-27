/**
 * @file credentials.h
 * @brief Persistent storage for device credentials, WiFi settings, and interval configuration.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <Arduino.h>

/**
 * @brief Saves device credentials to persistent storage.
 *
 * @param id              The unique device identifier.
 * @param secret          The device secret used for authentication.
 * @param provisionStatus Whether the device has been provisioned.
 */
void saveCredentials(String id, String secret, bool provisionStatus);

/**
 * @brief Loads device credentials from persistent storage.
 *
 * @param id              Reference to store the device identifier.
 * @param secret          Reference to store the device secret.
 * @param provisionStatus Reference to store the provisioning status.
 * @return true if both id and secret are non-empty, false otherwise.
 */
bool loadCredentials(String &id, String &secret, bool &provisionStatus);

/**
 * @brief Saves the measurement interval to persistent storage.
 *
 * @param interval Interval in milliseconds.
 */
void saveInterval(int interval);

/**
 * @brief Loads the measurement interval from persistent storage.
 *
 * @param interval Reference to store the loaded interval. Defaults to 10000ms if not set.
 */
void loadInterval(int &interval);

/**
 * @brief Saves WiFi credentials to persistent storage.
 *
 * @param ssid     The WiFi network name.
 * @param password The WiFi password.
 */
void saveWifi(String ssid, String password);

/**
 * @brief Loads WiFi credentials from persistent storage.
 *
 * @param ssid     Reference to store the WiFi network name.
 * @param password Reference to store the WiFi password.
 * @return true if both ssid and password are non-empty, false otherwise.
 */
bool loadWifi(String &ssid, String &password);

/**
 * @brief Clears all persistent storage including device, WiFi, and interval data.
 */
void clearAllData();

#endif