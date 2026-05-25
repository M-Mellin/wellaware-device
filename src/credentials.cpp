/**
 * @file credentials.cpp
 * @brief Persistent storage for device credentials, WiFi settings, and interval configuration.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include <Preferences.h>

Preferences prefs;

/**
 * @brief Saves device credentials to persistent storage.
 *
 * @param id              The unique device identifier.
 * @param secret          The device secret used for authentication.
 * @param provisionStatus Whether the device has been provisioned.
 */
void saveCredentials(String id, String secret, bool provisionStatus) {
  prefs.begin("device", false);
  prefs.putString("id", id);
  prefs.putString("secret", secret);
  prefs.putBool("provisioned", provisionStatus);
  prefs.end();
}

/**
 * @brief Saves the measurement interval to persistent storage.
 *
 * @param interval Interval in milliseconds.
 */
void saveInterval(int interval) {
  prefs.begin("device", false);
  prefs.putInt("interval", interval);
  prefs.end();
}

/**
 * @brief Loads the measurement interval from persistent storage.
 *
 * @param interval Reference to store the loaded interval. Defaults to 10000ms if not set.
 */
void loadInterval(int &interval) {
  prefs.begin("device", true);

  interval = prefs.getInt("interval", 10000);
  prefs.end();
}

/**
 * @brief Loads device credentials from persistent storage.
 *
 * @param id              Reference to store the device identifier.
 * @param secret          Reference to store the device secret.
 * @param provisionStatus Reference to store the provisioning status.
 * @return true if both id and secret are non-empty, false otherwise.
 */
bool loadCredentials(String &id, String &secret, bool &provisionStatus) {
  prefs.begin("device", true);

  id = prefs.getString("id", "");
  secret = prefs.getString("secret", "");
  provisionStatus = prefs.getBool("provisioned", false);

  prefs.end();

  return id.length() > 0 && secret.length() > 0;
}

/**
 * @brief Saves WiFi credentials to persistent storage.
 *
 * @param ssid     The WiFi network name.
 * @param password The WiFi password.
 */
void saveWifi(String ssid, String password) {
  prefs.begin("wifi", false);
  prefs.putString("ssid", ssid);
  prefs.putString("password", password);
  prefs.end();
}

/**
 * @brief Loads WiFi credentials from persistent storage.
 *
 * @param ssid     Reference to store the WiFi network name.
 * @param password Reference to store the WiFi password.
 * @return true if both ssid and password are non-empty, false otherwise.
 */
bool loadWifi(String &ssid, String &password) {
  prefs.begin("wifi", true);

  ssid = prefs.getString("ssid", "");
  password = prefs.getString("password", "");

  prefs.end();

  return ssid.length() > 0 && password.length() > 0;
}

/**
 * @brief Clears all persistent storage including device, WiFi, and interval data.
 */
void clearAllData() {
  prefs.begin("device", false);
  prefs.clear();
  prefs.end();

  prefs.begin("wifi", false);
  prefs.clear();
  prefs.end();

  prefs.begin("interval", false);
  prefs.clear();
  prefs.end();
}



