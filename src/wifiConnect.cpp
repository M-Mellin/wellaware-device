#include <Arduino.h>
#include <WiFi.h>

bool connectWifi(String ssid, String password) {
  WiFi.begin(ssid, password);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    Serial.println(ssid);
    Serial.println(password);
    return false;
  } else {
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }
}