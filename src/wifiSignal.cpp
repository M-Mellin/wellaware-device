/**
 * @file wifiSignal.cpp
 * @brief Utility for retrieving the current WiFi signal strength.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include <WiFi.h>
#include "wifiSignal.h"

/**
 * @brief Returns the current WiFi signal strength in dBm.
 *
 * @return int RSSI value in dBm. Closer to 0 is stronger (e.g. -50 is better than -90).
 *             Returns -100 if WiFi is not connected.
 */
int getWifiSignal() {
  if (WiFi.status() == WL_CONNECTED) {
    return WiFi.RSSI();
  } else {
    return -100;
  }
}