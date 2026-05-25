/**
 * @file ota.cpp
 * @brief Over-the-air firmware update management for the WellAware device.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include "ota.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Update.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "apiClient.h"

/**
 * @brief Retrieves the currently installed firmware version from persistent storage.
 *
 * @return String containing the saved version, or empty string if not set.
 */
static String getSavedVersion() {
  Preferences prefs;
  prefs.begin("ota", true);
  String version = prefs.getString("version", "");
  prefs.end();
  return version;
}

/**
 * @brief Saves the installed firmware version to persistent storage.
 *
 * @param version The version string to save.
 */
static void saveVersion(const String& version) {
  Preferences prefs;
  prefs.begin("ota", false);
  prefs.putString("version", version);
  prefs.end();
}

/**
 * @brief Downloads and installs a firmware binary from the given URL.
 *
 * Uses a secure HTTPS connection with TLS certificate validation.
 * Aborts the update if the download is incomplete or finalization fails.
 *
 * @param url  Full HTTPS URL to the firmware binary.
 * @return true if the firmware was written successfully, false otherwise.
 */
static bool performOta(const String& url) {
  WiFiClientSecure client;
  client.setCACert(ROOT_CA_CERT);

  HTTPClient http;
  http.begin(client, url);
  http.setTimeout(30000);
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("[OTA] Download failed, HTTP %d\n", httpCode);
    http.end();
    return false;
  }

  int contentLength = http.getSize();

  if (contentLength <= 0) {
    Serial.println("[OTA] Unknown content length");
    http.end();
    return false;
  }

  if (!Update.begin(contentLength)) {
    Serial.printf("[OTA] Not enough space: %s\n", Update.errorString());
    http.end();
    return false;
  }

  WiFiClient* stream = http.getStreamPtr();
  size_t written = Update.writeStream(*stream);
  http.end();

  if (written != (size_t)contentLength) {
    Serial.printf("[OTA] Written %d of %d bytes\n", written, contentLength);
    Update.abort();
    return false;
  }

  if (!Update.end()) {
    Serial.printf("[OTA] Failed to finalize: %s\n", Update.errorString());
    return false;
  }

  return true;
}

/**
 * @brief Checks the server for a newer firmware version and performs OTA if available.
 *
 * Compares the latest version from the server against the locally saved version.
 * Downloads and installs the update if a newer version is found, then restarts.
 *
 * @param deviceId  The unique device identifier.
 * @param jwtToken  JWT token for authentication.
 */
void checkForOtaUpdate(const String& deviceId, const String& jwtToken) {
  Serial.println("[OTA] Checking for update...");

  ApiClient client;
  ApiResponse res = client.get(String(API_BASE_URL) + "/ota/version", jwtToken);

  if (res.code != 200) {
    Serial.printf("[OTA] Version check failed, HTTP %d\n", res.code);
    return;
  }

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, res.body);

  if (err) {
    Serial.println("[OTA] JSON parse error");
    return;
  }

  String latestVersion = doc["version"].as<String>();
  String firmwareUrl   = doc["url"].as<String>();
  String savedVersion  = getSavedVersion();

  Serial.printf("[OTA] Saved: %s  Latest: %s\n", savedVersion.c_str(), latestVersion.c_str());

  if (latestVersion == savedVersion) {
    Serial.println("[OTA] Already up to date");
    return;
  }

  Serial.printf("[OTA] New version %s found, downloading...\n", latestVersion.c_str());

  if (performOta(firmwareUrl)) {
    saveVersion(latestVersion);
    Serial.println("[OTA] Update complete, restarting...");
    delay(500);
    ESP.restart();
  }
}