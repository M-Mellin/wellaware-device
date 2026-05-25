#ifndef SETUP_MODE_H
#define SETUP_MODE_H
#include <Arduino.h>
#include "../config.h"

void handleSetup();

bool startSetupMode();
bool setDeviceProvisioning(String deviceId, String token);

#endif