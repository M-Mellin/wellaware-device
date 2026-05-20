#include <Arduino.h>
#include <WiFi.h>
#include "floatSensor.h"
#include "ultrasonicSensor.h"
#include "wifiConnect.h"
#include "httpRequest.h"
#include "wifiSignal.h"
#include "syncTime.h"
#include "localData.h"
#include "deviceId.h"
#include <esp_system.h>
#include "credentials.h"
#include "setupMode.h"
#include "fetchToken.h"
#include "commands.h"

const int ECHO_PIN = 4;
const int TRIG_PIN = 5;

String wifiSsid;
String wifiPassword;

String deviceId;
String deviceSecret;
bool provisionStatus = false;

int deviceInterval = 3000;

String jwtToken;

unsigned long lastTokenRefresh = 0;

const unsigned long TOKEN_REFRESH_INTERVAL = 3600000;

const int MAX_MEASUREMENTS = 500;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

bool timeInitialized = false;

Measurement pendingMeasurements[MAX_MEASUREMENTS];
int measurementCount = 0;

void handleWifiConnection() {

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi lost. Reconnecting...");

    connectWifi(wifiSsid, wifiPassword);
  }
}

void refreshTokenIfNeeded() {
  if (jwtToken.isEmpty() || millis() - lastTokenRefresh > TOKEN_REFRESH_INTERVAL) {
    Serial.println("Refreshing JWT token...");

    jwtToken = fetchToken(deviceId, deviceSecret);

    if (jwtToken.isEmpty()) {
      Serial.println("Failed to refresh JWT token");
    } else {
      Serial.println("JWT token refreshed successfully");
      lastTokenRefresh = millis();
    }
  }
}

void handleMeasurements() {
  float distance = readUltrasonicSensor(ECHO_PIN, TRIG_PIN);

  unsigned long unixTimestamp = time(nullptr);

  addMeasurement(pendingMeasurements, distance, unixTimestamp, measurementCount, MAX_MEASUREMENTS);
}

void handleMeasurementUploads() {
  if (WiFi.status() != WL_CONNECTED || jwtToken.isEmpty()) {
    return;
  }

  float signal = getWifiSignal();

  Serial.println("Trying to send pending measurements...");

  bool success = sendMessage(pendingMeasurements, measurementCount, signal, deviceId, jwtToken);

  if (success) {
    clearMeasurements(measurementCount);

    Serial.println("SUCESS: Measurements have been sent.");
  } else {
    Serial.println("FAILED: Send failed, local data is kept.");
  }
}

void handleCommands() {

  if (WiFi.status() != WL_CONNECTED || jwtToken.isEmpty()) {
    return;
  }

  bool shouldEnterSetupMode = checkForCommands(deviceId, jwtToken);

  loadInterval(deviceInterval);

  if (shouldEnterSetupMode) {

    Serial.println("Entering setup mode...");

    startSetupMode();
  }
}

void setup() {
  Serial.begin(115200);

  bool hasCredentials = loadCredentials(deviceId, deviceSecret, provisionStatus);
  bool hasWifi = loadWifi(wifiSsid, wifiPassword);

  if (!hasCredentials || !hasWifi) {
    startSetupMode();
    return;
  }

  loadInterval(deviceInterval);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  connectWifi(wifiSsid, wifiPassword);

  while (!timeInitialized) {
    if (WiFi.status() == WL_CONNECTED) {
      timeInitialized = syncTime(ntpServer, gmtOffset_sec, daylightOffset_sec);
    }

    if (!timeInitialized) {
      Serial.println("Watiing for valid time...");
      delay(5000);

      if (WiFi.status() != WL_CONNECTED) {
        connectWifi(wifiSsid, wifiPassword);
      }
    }
  }
  
  jwtToken = fetchToken(deviceId, deviceSecret);

  lastTokenRefresh = millis();

  if (!provisionStatus) {

    int retries = 0;
    bool success = false;

    while (!success && retries < 10) {

      success = setDeviceProvisioning(deviceId, jwtToken);

      if (!success) {
        retries++;
        delay(5000);
      }
    }

    if (success) {
      saveCredentials(deviceId, deviceSecret, true);
      provisionStatus = true;
    }
  }
}

void loop() {

  handleWifiConnection();

  refreshTokenIfNeeded();

  handleMeasurements();

  handleMeasurementUploads();

  handleCommands();

  delay(deviceInterval);
}