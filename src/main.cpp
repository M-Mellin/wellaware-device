/**
 * @file main.cpp
 * @brief Main entry point for the WellAware device firmware.
 *
 * Handles device initialization, WiFi connectivity, sensor measurements,
 * data uploads, command processing, and OTA updates.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include <Arduino.h>
#include <WiFi.h>
#include <LittleFS.h>
#include "sensorStrategies/sensorProcessor.h"
#include "sensorStrategies/ultrasonicSensor.h"
#include "sensorStrategies/medianCalculation.h"
#include "calculationFactory.h"
#include "calculationStore.h"
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
#include "ota.h"
#include "../config.h"

String wifiSsid;
String wifiPassword;

String deviceId;
String deviceSecret;
bool provisionStatus = false;

int deviceInterval = 3000;

String jwtToken;

unsigned long lastTokenRefresh = 0;

bool timeInitialized = false;

Measurement pendingMeasurements[MAX_MEASUREMENTS];
int measurementCount = 0;

SensorProcessor* processor = nullptr;

void checkForNewVersion();
void handleWifiConnection();
void refreshTokenIfNeeded();
void handleMeasurements();
void handleMeasurementUploads();
void handleCommands();
void setCalculationMethod(const String& method);

/**
 * @brief Initializes the device, loads credentials, connects to WiFi,
 *        syncs time, fetches a JWT token, and provisions the device if needed.
 */
void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS failed to mount!");
  } else {
    Serial.println("LittleFS mounted successfully.");
  }

  bool hasCredentials = loadCredentials(deviceId, deviceSecret, provisionStatus);
  bool hasWifi = loadWifi(wifiSsid, wifiPassword);

  if (!hasCredentials || !hasWifi) {
    startSetupMode();
    return;
  }

  loadInterval(deviceInterval);

  loadMeasurementsFromFlash(pendingMeasurements, measurementCount, MAX_MEASUREMENTS);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  connectWifi(wifiSsid, wifiPassword);

  unsigned long wifiStart = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - wifiStart < 10000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  while (!timeInitialized) {
    if (WiFi.status() == WL_CONNECTED) {
      timeInitialized = syncTime();
    }

    if (!timeInitialized) {
      Serial.println("Waiting for valid time...");
      delay(5000);

      if (WiFi.status() != WL_CONNECTED) {
        connectWifi(wifiSsid, wifiPassword);
      }
    }
  }
  
  jwtToken = fetchToken(deviceId, deviceSecret);

  if (jwtToken.isEmpty()) {
    Serial.println("Failed to fetch token, skipping OTA check");
  } else {
    checkForOtaUpdate(deviceId, jwtToken);
  }

  String method = loadCalculationMethod();
  setCalculationMethod(method);

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

/**
 * @brief Main loop. Runs continuously to handle connectivity,
 *        measurements, uploads, commands, and OTA checks.
 */
void loop() {
  checkForNewVersion();

  handleWifiConnection();

  refreshTokenIfNeeded();

  handleMeasurements();

  handleMeasurementUploads();

  handleCommands();

  delay(deviceInterval);
}

/**
 * @brief Checks for a new OTA firmware version at a fixed interval.
 *
 * Only triggers a check every OTA_CHECK_INTERVAL milliseconds.
 */
void checkForNewVersion() {
  static unsigned long lastOtaCheck = 0;

  if (millis() - lastOtaCheck >= OTA_CHECK_INTERVAL) {
    lastOtaCheck = millis();
    checkForOtaUpdate(deviceId, jwtToken);
  }
}

/**
 * @brief Reconnects to WiFi if the connection has been lost.
 */
void handleWifiConnection() {

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi lost. Reconnecting...");

    connectWifi(wifiSsid, wifiPassword);
  }
}

/**
 * @brief Refreshes the JWT token if it is empty or has expired.
 */
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

/**
 * @brief Reads the ultrasonic sensor and adds the measurement to the buffer.
 */
void handleMeasurements() {
  float distance = processor->read();

  if (distance < 0) {
    return;
  }

  unsigned long unixTimestamp = time(nullptr);

  addMeasurement(pendingMeasurements, distance, unixTimestamp, measurementCount, MAX_MEASUREMENTS);
}

/**
 * @brief Uploads pending measurements to the server if WiFi and token are available.
 *
 * Clears the local buffer on success.
 */
void handleMeasurementUploads() {
  if (WiFi.status() != WL_CONNECTED || jwtToken.isEmpty()) {
    return;
  }

  float signal = getWifiSignal();

  Serial.println("Trying to send pending measurements...");

  bool success = sendMessage(pendingMeasurements, measurementCount, signal, deviceId, jwtToken);

  if (success) {
    clearMeasurements(measurementCount);

    Serial.println("SUCCESS: Measurements have been sent.");
  } else {
    Serial.println("FAILED: Send failed, local data is kept.");
  }
}

/**
 * @brief Fetches and processes pending commands from the server.
 *
 * Enters setup mode if the server requests it.
 */
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

void setCalculationMethod(const String& method) {
    delete processor;
    processor = new SensorProcessor(
        new UltrasonicSensor(TRIG_PIN, ECHO_PIN),
        createCalculation(method),
        MEDIAN_SAMPLES
    );
    Serial.println("[Sensor] Calculation method set to: " + method);
}