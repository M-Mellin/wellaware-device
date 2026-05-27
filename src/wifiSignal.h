/**
 * @file wifiSignal.h
 * @brief Utility for retrieving the current WiFi signal strength.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */
#pragma once

/**
 * @brief Returns the current WiFi signal strength in dBm.
 *
 * @return int RSSI value in dBm. Closer to 0 is stronger (e.g. -50 is better than -90).
 */
int getWifiSignal();