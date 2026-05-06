#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <Arduino.h>

struct Measurement {
  float level;
  unsigned long timestamp;
};

bool sendMessage(
  Measurement measurements[],
  int count,
  float signal,
  const String& deviceId,
  const String& secret
);

#endif