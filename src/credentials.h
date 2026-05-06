#ifndef CREDENTIALS_H
#define CREDENTIALS_H
#include <Arduino.h>

void saveCredentials(String id, String secret, int interval);
bool loadCredentials(String &id, String &secret, int interval);
void saveWifi(String ssid, String password);
bool loadWifi(String &ssid, String &password);
#endif