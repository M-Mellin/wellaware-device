/**
 * @file localData.cpp
 * @brief Local storage management for measurements using LittleFS.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include "localData.h"

static unsigned long lastFlashSave = 0;

/**
 * @brief Adds a measurement to the buffer and saves to flash if WiFi is disconnected.
 *
 * @param pendingMeasurements  Array to store measurements in.
 * @param level                The measured level value.
 * @param timestamp            Unix timestamp of the measurement.
 * @param measurementCount     Reference to the current measurement count.
 * @param maxMeasurements      Maximum number of measurements the buffer can hold.
 */
void addMeasurement(Measurement pendingMeasurements[], float level, unsigned long timestamp, int& measurementCount, int maxMeasurements) {
  if (measurementCount >= maxMeasurements) {
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

/**
 * @brief Clears all measurements from the buffer and removes the flash file.
 *
 * @param measurementCount Reference to the measurement count, reset to 0.
 */
void clearMeasurements(int& measurementCount) {
  measurementCount = 0;
  if (LittleFS.exists(MEASUREMENTS_FILE)) {
    LittleFS.remove(MEASUREMENTS_FILE);
  }
  Serial.println("Measurements cleared.");
}

/**
 * @brief Saves the current measurement buffer to flash storage.
 *
 * @param pendingMeasurements  Array of measurements to save.
 * @param measurementCount     Number of measurements to save.
 * @return true if saved successfully, false otherwise.
 */
bool saveMeasurementsToFlash(Measurement pendingMeasurements[], int measurementCount) {
  File file = LittleFS.open(MEASUREMENTS_FILE, "w");
  if (!file) {
    Serial.println("Failed to open measurements file for writing.");
    return false;
  }

  file.write(reinterpret_cast<uint8_t*>(&measurementCount), sizeof(measurementCount));
  file.write(reinterpret_cast<uint8_t*>(pendingMeasurements), sizeof(Measurement) * measurementCount);
  file.close();

  Serial.println("Saved to flash.");
  return true;
}

/**
 * @brief Loads measurements from flash storage into the buffer.
 *
 * @param pendingMeasurements  Array to load measurements into.
 * @param measurementCount     Reference to store the number of loaded measurements.
 * @param maxMeasurements      Maximum number of measurements the buffer can hold.
 * @return true if measurements were loaded successfully, false otherwise.
 */
bool loadMeasurementsFromFlash(Measurement pendingMeasurements[], int& measurementCount, int maxMeasurements) {
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
  file.read(reinterpret_cast<uint8_t*>(&storedCount), sizeof(storedCount));

  if (storedCount < 0 || storedCount > maxMeasurements) {
    Serial.println("Corrupt measurement file. Discarding.");
    file.close();
    LittleFS.remove(MEASUREMENTS_FILE);
    return false;
  }

  file.read(reinterpret_cast<uint8_t*>(pendingMeasurements), sizeof(Measurement) * storedCount);
  file.close();

  measurementCount = storedCount;
  Serial.print("Loaded ");
  Serial.print(measurementCount);
  Serial.println(" measurements from flash.");

  return measurementCount > 0;
}