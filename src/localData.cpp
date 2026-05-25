#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include "localData.h"

static unsigned long lastFlashSave = 0;

void addMeasurement(Measurement pendingMeasurements[], float level, unsigned long timestamp, int& measurementCount, int MAX_MEASUREMENTS) {
  if (measurementCount >= MAX_MEASUREMENTS) {
    Serial.println("Buffer full. Measurement skipped.");
    return;
  }

  pendingMeasurements[measurementCount].level = level;
  pendingMeasurements[measurementCount].timestamp = timestamp;
  measurementCount++;

  if (WiFi.status() != WL_CONNECTED) {
    if (millis() - lastFlashSave >= FLASH_SAVE_INTERVAL) {
      saveMeasurementsToFlash(pendingMeasurements, measurementCount);
      lastFlashSave = millis();
    }
  }

  Serial.print("Saved locally. Count: ");
  Serial.println(measurementCount);
}

void clearMeasurements(int& measurementCount) {
  measurementCount = 0;
  if (LittleFS.exists(MEASUREMENTS_FILE)) {
    LittleFS.remove(MEASUREMENTS_FILE);
  }
  Serial.println("Measurements cleared.");
}

bool saveMeasurementsToFlash(Measurement pendingMeasurements[], int measurementCount) {
  File file = LittleFS.open(MEASUREMENTS_FILE, "w");
  if (!file) {
    Serial.println("Failed to open measurements file for writing.");
    return false;
  }

  file.write((uint8_t*)&measurementCount, sizeof(measurementCount));
  file.write((uint8_t*)pendingMeasurements, sizeof(Measurement) * measurementCount);
  file.close();

  Serial.println("Saved to flash.");
  return true;
}

bool loadMeasurementsFromFlash(Measurement pendingMeasurements[], int& measurementCount, int MAX_MEASUREMENTS) {
  if (!LittleFS.exists(MEASUREMENTS_FILE)) {
    Serial.println("No stored measurements found.");
    return false;
  }

  File file = LittleFS.open(MEASUREMENTS_FILE, "r");
  if (!file) {
    Serial.println("Failed to open measurements file for reading.");
    return false;
  }

  int storedCount = 0;
  file.read((uint8_t*)&storedCount, sizeof(storedCount));

  if (storedCount < 0 || storedCount > MAX_MEASUREMENTS) {
    Serial.println("Corrupt measurement file. Discarding.");
    file.close();
    LittleFS.remove(MEASUREMENTS_FILE);
    return false;
  }

  file.read((uint8_t*)pendingMeasurements, sizeof(Measurement) * storedCount);
  file.close();

  measurementCount = storedCount;
  Serial.print("Loaded ");
  Serial.print(measurementCount);
  Serial.println(" measurements from flash.");

  return measurementCount > 0;
}