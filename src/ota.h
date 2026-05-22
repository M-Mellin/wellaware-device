#ifndef OTA_H
#define OTA_H

#include <Arduino.h>

void checkForOtaUpdate(const String& deviceId, const String& jwtToken);

#endif