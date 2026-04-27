#include <Arduino.h>
#include <WiFi.h>

bool connectWifi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    return false;
  } else {
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }
}