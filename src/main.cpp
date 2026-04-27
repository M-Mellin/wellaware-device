#include <Arduino.h>
#include <WiFi.h>
#include "floatSensor.h"
#include "ultrasonicSensor.h"
#include "wifiConnect.h"
#include "httpRequest.h"
#include "wifiSignal.h"

const int FLOAT_PIN = A1;
const int ECHO_PIN = 4;
const int TRIG_PIN = 5;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const String DEVICE_ID = "Device-X1";

const int MAX_MEASUREMENTS = 500;

Measurement pendingMeasurements[MAX_MEASUREMENTS];
int measurementCount = 0;

void addMeasurement(float level, unsigned long timestamp) {
  if (measurementCount >= MAX_MEASUREMENTS) {
    Serial.println("Buffer full. Measurment skipped.");
    return;
  }

  pendingMeasurements[measurementCount].level = level;
  pendingMeasurements[measurementCount].timestamp = timestamp;

  measurementCount++;

  Serial.print("Saved locally. Count: ");
  Serial.println(measurementCount);
}

void clearMeasurements() {
  measurementCount = 0;
  Serial.println("Measurement cleared.");
}

void setup() {
  Serial.begin(115200);

  pinMode(FLOAT_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  connectWifi(ssid, password);
}

void loop() {
  float distance = readUltrasonicSensor(ECHO_PIN, TRIG_PIN);

  addMeasurement(distance, millis());

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost. Reconnecting...");
    connectWifi(ssid, password);
  } else {
    float signal = getWifiSignal();
    bool isClosed = readFloatSensor(FLOAT_PIN);

    const String floatState = isClosed ? "CLOSED" : "OPEN";

    Serial.println("Trying to send pending measurements...");

    bool success = sendMessage(
      pendingMeasurements,
      measurementCount,
      signal,
      floatState,
      DEVICE_ID
    );

    if (success) {
      clearMeasurements();
      Serial.println("SUCESS: Measurements has been sent.");
    } else {
      Serial.println("Send failed. Local data is saved.");
    }
  }

  delay(1000);
}