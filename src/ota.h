#ifndef OTA_H
#define OTA_H

#include <Arduino.h>
#include "../config.h"

void checkForOtaUpdate(const String& deviceId, const String& jwtToken);

#endif