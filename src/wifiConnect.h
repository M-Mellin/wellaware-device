/**
 * @file wifiConnect.h
 * @brief WiFi connection management for the WellAware device.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#pragma once

#include <Arduino.h>

/**
 * @brief Attempts to connect to a WiFi network.
 *
 * Initiates the connection and immediately checks the status.
 * Note: since WiFi.begin() is non-blocking, this may return false
 * even if the connection eventually succeeds. Consider polling
 * WiFi.status() in the calling code if a reliable connection is needed.
 *
 * @param ssid      The WiFi network name.
 * @param password  The WiFi network password.
 * @return true if already connected, false if connection is still in progress.
 */
bool connectWifi(String ssid, String password);