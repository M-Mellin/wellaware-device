#include <WiFi.h>

int getWifiSignal() {
  if (WiFi.status() == WL_CONNECTED) {
    return WiFi.RSSI();
  } else {
    return -100; // fallback
  }
}