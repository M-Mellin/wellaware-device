#include <Arduino.h>
#include <WiFi.h>

bool connectWifi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  return true;
}