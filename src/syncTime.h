#ifndef SYNC_TIME_H
#define SYNC_TIME_H

bool syncTime(const char* ntpServer, const long gmtOffset_sec, const long daylightOffset_sec);

#endif