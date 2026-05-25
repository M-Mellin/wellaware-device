/**
 * @file syncTime.h
 * @brief NTP time synchronization for the WellAware device.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#ifndef SYNC_TIME_H
#define SYNC_TIME_H

#include "../config.h"

/**
 * @brief Synchronizes the device clock with an NTP server.
 *
 * Configures the system time using the provided NTP server and offsets,
 * then verifies that a valid time has been obtained.
 *
 * @param ntpServer          Hostname of the NTP server (e.g. "pool.ntp.org").
 * @param gmtOffset_sec      GMT offset in seconds (e.g. 3600 for UTC+1).
 * @param daylightOffset_sec Daylight saving time offset in seconds (e.g. 3600).
 * @return true if time was synchronized successfully, false otherwise.
 */
bool syncTime();

#endif