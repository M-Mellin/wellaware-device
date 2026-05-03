#include <Arduino.h>
#include "deviceId.h"

String getDeviceId() {
  uint64_t chipId = ESP.getEfuseMac();

  char deviceId[32];
  snprintf(
    deviceId,
    sizeof(deviceId),
    "wellaware-%04X%08X",
    (uint16_t)(chipId >> 32),
    (uint32_t)chipId
  );

  return String(deviceId);
}