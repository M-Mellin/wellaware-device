#ifndef CREDENTIALS_H
#define CREDENTIALS_H
#include <Arduino.h>

void saveCredentials(String id, String secret, bool provisionStatus);
bool loadCredentials(String &id, String &secret, bool &provisionStatus);
void saveInterval(int interval);
void loadInterval(int &interval);
void saveWifi(String ssid, String password);
bool loadWifi(String &ssid, String &password);
void clearAllData();
#endif