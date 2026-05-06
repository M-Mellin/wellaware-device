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

const int FLOAT_PIN = A1;
const int ECHO_PIN = 4;
const int TRIG_PIN = 5;

String wifiSsid;
String wifiPassword;

String deviceId;
String deviceSecret;
String jwtToken;
unsigned long tokenExpiry = 0;

const int MAX_MEASUREMENTS = 500;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

bool timeInitialized = false;

Measurement pendingMeasurements[MAX_MEASUREMENTS];
int measurementCount = 0;

void setup() {
  Serial.begin(115200);

  bool hasCredentials = loadCredentials(deviceId, deviceSecret);
  bool hasWifi = loadWifi(wifiSsid, wifiPassword);

  if (!hasCredentials || !hasWifi) {
    startSetupMode();
    return;
  }

  pinMode(FLOAT_PIN, INPUT_PULLUP);
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
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost. Reconnecting...");
    connectWifi(wifiSsid, wifiPassword);
  }

  float distance = readUltrasonicSensor(ECHO_PIN, TRIG_PIN);
  bool isClosed = readFloatSensor(FLOAT_PIN);
  float signal = getWifiSignal();

  const String floatState = isClosed ? "CLOSED" : "OPEN";

  unsigned long unixTimestamp = time(nullptr);

  addMeasurement(pendingMeasurements, distance, unixTimestamp, measurementCount, MAX_MEASUREMENTS);

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Trying to send pending measurements...");

    bool success = sendMessage(
      pendingMeasurements,
      measurementCount,
      signal,
      floatState,
      deviceId
    );

    if (success) {
      clearMeasurements(measurementCount);
      Serial.println("SUCCESS: Measurements have been sent.");
    } else {
      Serial.println("Send failed. Local data is kept.");
    }
  }
  delay(3000);
}