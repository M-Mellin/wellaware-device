#include "ota.h"
#include <HTTPClient.h>
#include <Update.h>
#include <ArduinoJson.h>
#include <Preferences.h>

const char* OTA_VERSION_URL = "https://mellin.net/wellaware/api/v1/ota/version";

static String getSavedVersion() {
  Preferences prefs;
  prefs.begin("ota", true);
  String version = prefs.getString("version", "");
  prefs.end();
  return version;
}

static void saveVersion(const String& version) {
  Preferences prefs;
  prefs.begin("ota", false);
  prefs.putString("version", version);
  prefs.end();
}

static bool performOta(const String& url) {
  HTTPClient http;
  http.begin(url);
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

  if (written != contentLength) {
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

void checkForOtaUpdate(const String& deviceId, const String& jwtToken) {
  Serial.println("[OTA] Checking for update...");

  HTTPClient http;
  http.begin(OTA_VERSION_URL);
  http.addHeader("Authorization", "Bearer " + jwtToken);
  http.setTimeout(10000);

  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("[OTA] Version check failed, HTTP %d\n", httpCode);
    http.end();
    return;
  }

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, http.getString());
  http.end();

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