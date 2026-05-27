/**
 * @file localData.h
 * @brief Local storage management for measurements using LittleFS.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#pragma once

#include <Arduino.h>
#include "../config.h"

/**
 * @brief Represents a single sensor measurement.
 */
struct Measurement {
  float level;          ///< The measured level value.
  unsigned long timestamp; ///< Unix timestamp of the measurement.
};

/**
 * @brief Adds a measurement to the buffer and saves to flash if WiFi is disconnected.
 *
 * @param pendingMeasurements  Array to store measurements in.
 * @param level                The measured level value.
 * @param timestamp            Unix timestamp of the measurement.
 * @param measurementCount     Reference to the current measurement count.
 * @param maxMeasurements      Maximum number of measurements the buffer can hold.
 */
void addMeasurement(Measurement pendingMeasurements[], float level, unsigned long timestamp, int& measurementCount, int maxMeasurements);

/**
 * @brief Clears all measurements from the buffer and removes the flash file.
 *
 * @param measurementCount Reference to the measurement count, reset to 0.
 */
void clearMeasurements(int& measurementCount);

/**
 * @brief Saves the current measurement buffer to flash storage.
 *
 * @param pendingMeasurements  Array of measurements to save.
 * @param measurementCount     Number of measurements to save.
 * @return true if saved successfully, false otherwise.
 */
bool saveMeasurementsToFlash(Measurement pendingMeasurements[], int measurementCount);

/**
 * @brief Loads measurements from flash storage into the buffer.
 *
 * @param pendingMeasurements  Array to load measurements into.
 * @param measurementCount     Reference to store the number of loaded measurements.
 * @param maxMeasurements      Maximum number of measurements the buffer can hold.
 * @return true if measurements were loaded successfully, false otherwise.
 */
bool loadMeasurementsFromFlash(Measurement pendingMeasurements[], int& measurementCount, int maxMeasurements);