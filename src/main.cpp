#include <Arduino.h>
#include <WiFi.h>
#include "floatSensor.h"
#include "ultrasonicSensor.h"
#include "wifiConnect.h"
#include "httpRequest.h"
#include "wifiSignal.h"
#include "syncTime.h"
#include "localData.h"

const int FLOAT_PIN = A1;
const int ECHO_PIN = 4;
const int TRIG_PIN = 5;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const String DEVICE_ID = "Device-X1";

const int MAX_MEASUREMENTS = 500;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

bool timeInitialized = false;

Measurement pendingMeasurements[MAX_MEASUREMENTS];
int measurementCount = 0;

void setup() {
  Serial.begin(115200);

  pinMode(FLOAT_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  connectWifi(ssid, password);

  while (!timeInitialized) {
    if (WiFi.status() == WL_CONNECTED) {
      timeInitialized = syncTime(ntpServer, gmtOffset_sec, daylightOffset_sec);
    }

    if (!timeInitialized) {
      Serial.println("Watiing for valid time...");
      delay(5000);

      if (WiFi.status() != WL_CONNECTED) {
        connectWifi(ssid, password);
      }
    }
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost. Reconnecting...");
    connectWifi(ssid, password);
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
      DEVICE_ID
    );

    if (success) {
      clearMeasurements(measurementCount);
      Serial.println("SUCCESS: Measurements have been sent.");
    } else {
      Serial.println("Send failed. Local data is kept.");
    }
  }
  delay(5000);
}