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

void setup() {
  Serial.begin(115200);

  pinMode(FLOAT_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  connectWifi(ssid, password);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost. Reconnecting...");
    connectWifi(ssid, password);
  } else {
    float distance = readUltrasonicSensor(ECHO_PIN, TRIG_PIN);
    bool isOpen = readFloatSensor(FLOAT_PIN);
    float signal = getWifiSignal();

    const char* floatState = isOpen ? "OPEN" : "CLOSED";

    Serial.println("Sending...");
    unsigned long start = millis();

    sendMessage(distance, signal, floatState, "Device-X1");

    Serial.print("Time: ");
    Serial.println(millis() - start);
    Serial.println("Sent!");
  }

  delay(10000);
}