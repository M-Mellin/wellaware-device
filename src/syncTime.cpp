/**
 * @file syncTime.cpp
 * @brief NTP time synchronization for the WellAware device.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include <Arduino.h>
#include "syncTime.h"

/**
 * @brief Synchronizes the device clock with an NTP server.
 *
 * Configures the system time using the provided NTP server and offsets,
 * then verifies that a valid time has been obtained.
 *
 * @param ntpServer          Hostname of the NTP server (e.g. "pool.ntp.org").
 * @param gmtOffset_sec      GMT offset in seconds (e.g. 3600 for UTC+1).
 * @param daylightOffset_sec Daylight saving time offset in seconds (e.g. 3600).
 * @return true if time was synchronized successfully, false otherwise.
 */
bool syncTime(const char* ntpServer, const long gmtOffset_sec, const long daylightOffset_sec) {
  Serial.println("Syncing time with NTP...");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return false;
  }

  Serial.println("Time synchronized successfully.");
  return true;
}