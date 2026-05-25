#include <Arduino.h>
#include "syncTime.h"

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