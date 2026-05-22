#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <Arduino.h>
#include "localData.h"

bool sendMessage(
  Measurement measurements[],
  int count,
  float signal,
  const String& deviceId,
  const String& secret
);

#endif