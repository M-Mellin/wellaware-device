# WellAware IoT Device

ESP32 firmware for the WellAware water meter device. Measures water levels using an ultrasonic sensor, uploads data to the WellAware API, and supports over-the-air (OTA) firmware updates.
 
Built with **C++** and the **Arduino framework** for ESP32.

---
 
## Table of Contents
 
- [Prerequisites](#prerequisites)
- [Configuration](#configuration)
- [CI/CD](#cicd)
- [Flashing the Device](#flashing-the-device)
- [Device Setup](#device-setup)
- [Architecture](#architecture)
- [Main Loop](#main-loop)
- [Modules](#modules)

---

## Prerequisites
 
- [PlatformIO](https://platformio.org/) (CLI or VS Code extension)
- ESP32 development board
- Ultrasonic distance sensor (e.g. HC-SR04)
- TLS root CA certificate for your API domain

---
 
## Configuration
 
`config.h` is **not tracked by git** and is generated automatically by the CI/CD pipeline during builds. For local development, create it manually in the project root:
 
```cpp
#pragma once
 
// Server
#define API_BASE_URL        "https://your-api-domain.com/api/v1"
#define API_TIMEOUT_MS      5000
 
// Setup mode
#define SETUP_AP_SSID       "WellAware-Device-Setup"
#define SETUP_TIMEOUT_MS    30000                       // 30 seconds
 
// Local data
#define FLASH_SAVE_INTERVAL 15 * 60 * 1000             // 15 minutes
#define MEASUREMENTS_FILE   "/measurements.bin"
#define MAX_MEASUREMENTS    500

// Calculation method
#define DEFAULT_CALCULATION_METHOD "validation"
#define CALCULATION_METHOD_FILE    "/calculation_method.txt"
#define VALIDATION_MIN_INTERVAL_SEC 60
 
// Ultrasonic sensor
#define ECHO_PIN            4
#define TRIG_PIN            5
#define MEDIAN_SAMPLES      12
 
// Token refresh
#define TOKEN_REFRESH_INTERVAL  3600000                 // 1 hour
 
// OTA
#define OTA_CHECK_INTERVAL  6UL * 60UL * 60UL * 1000UL // 6 hours
 
// NTP
#define NTP_SERVER          "pool.ntp.org"
#define GMT_OFFSET_SEC      0
#define DAYLIGHT_OFFSET_SEC 0
 
// TLS root CA certificate (Let's Encrypt E8)
static const char ROOT_CA_CERT[] = R"(
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
)";
```
 
The full certificate is available from [Let's Encrypt](https://letsencrypt.org/certificates/) or can be obtained by running:
```
openssl s_client -connect your-api-domain.com:443 -showcerts
```
 
---

## CI/CD
 
The project uses **GitLab CI** with two stages: `build → release`.
 
| Stage | Description |
|-------|-------------|
| `build-firmware` | Installs PlatformIO, generates `config.h` from CI variables, and compiles the firmware |
| `release-firmware` | Computes SHA256, base64-encodes the binary, and registers the release via `POST /api/v1/ota/release` |
 
Both stages trigger only on tags matching `v*` (e.g. `v1.2.0`). The tag is used as the firmware version number.
 
**Required CI/CD variables** (set in GitLab project settings):
 
| Variable | Description |
|----------|-------------|
| `API_BASE_URL` | Base URL of the WellAware API |
| `ROOT_CA_CERT` | Base64-encoded TLS root CA certificate |
| `OTA_ADMIN_SECRET` | Admin secret for the OTA release endpoint |
 
`config.h` is generated entirely by the pipeline — it does not need to exist in the repository.
 
---


## Flashing the Device
 
```bash
# Build and flash
pio run --target upload
 
# Monitor serial output
pio device monitor --baud 115200
```
 
---
 
## Device Setup
 
On first boot — or after a factory reset — the device starts in **setup mode**:
 
1. The ESP32 creates a WiFi access point named `WellAware-Setup`.
2. Connect to the access point and send a POST request to `http://192.168.4.1/setup`:
```json
{
  "deviceId": "wellaware-XXXXXXXXXXXX",
  "secret": "your-device-secret",
  "ssid": "YourWiFiNetwork",
  "password": "YourWiFiPassword",
  "interval": 3600
}
```
 
`interval` is the measurement reporting interval in seconds (5–3600).
 
The device ID and secret are obtained when registering the device via the WellAware app or API (`POST /api/v1/users/me/devices`).
 
3. On success, the device saves the credentials to persistent storage and restarts.
4. Setup mode automatically times out and restarts after 5 minutes if no configuration is received.
---
 
## Architecture
 
```
wellaware-firmware/
├── src/
│   ├── main.cpp                # Entry point, setup() and loop()
│   ├── apiClient.cpp/.h        # HTTPS client (GET, POST, PATCH, DELETE)
│   ├── commands.cpp/.h         # Command polling and handling
│   ├── credentials.cpp/.h      # Persistent storage (NVS) for credentials and settings
│   ├── deviceId.cpp/.h         # Unique device ID derived from ESP32 MAC address
│   ├── fetchToken.cpp/.h       # JWT token retrieval from the API
│   ├── httpRequest.cpp/.h      # Measurement upload
│   ├── localData.cpp/.h        # Offline measurement buffer (LittleFS)
│   ├── ota.cpp/.h              # OTA firmware update
│   ├── setupMode.cpp/.h        # Setup mode AP and HTTP server
│   ├── syncTime.cpp/.h         # NTP time synchronization
│   ├── ultrasonicSensor.cpp/.h # Sensor driver with median filtering
│   ├── wifiConnect.cpp/.h      # WiFi connection management
│   └── wifiSignal.cpp/.h       # WiFi RSSI utility
└── config.h                    # Local configuration (not tracked by git)
```
 
### Key Design Decisions
 
- **Device ID** — derived from the ESP32 eFuse MAC address, formatted as `wellaware-XXXXYYYYYYYY`. Guaranteed unique per chip, no configuration required.
- **Offline buffering** — measurements are stored in a RAM buffer. When WiFi is unavailable, the buffer is periodically flushed to LittleFS flash storage and uploaded once connectivity is restored.
- **Median filtering** — the ultrasonic sensor takes multiple samples per reading and returns the median, reducing the impact of spurious echo readings.
- **JWT refresh** — the device token is refreshed automatically every 55 minutes, before the 1-hour expiry.
- **OTA** — on startup and periodically during operation, the device checks the API for a newer firmware version. If one is found, it downloads and installs the binary over HTTPS and restarts.
- **Persistent storage** — credentials, WiFi settings, and measurement interval are stored in ESP32 NVS (Non-Volatile Storage) via the `Preferences` library and survive power cycles.
---
 
## Main Loop
 
Each iteration of `loop()` runs the following steps in order:
 
```
1. checkForNewVersion()       — OTA check (throttled to OTA_CHECK_INTERVAL)
2. handleWifiConnection()     — reconnect if WiFi was lost
3. refreshTokenIfNeeded()     — refresh JWT if expired
4. handleMeasurements()       — read ultrasonic sensor, add to buffer
5. handleMeasurementUploads() — send buffered measurements to API
6. handleCommands()           — poll and process pending server commands
7. delay(deviceInterval)      — wait until next measurement cycle
```
 
---
 
## Modules
 
### `ultrasonicSensor`
Takes `MEDIAN_SAMPLES` readings from the HC-SR04 sensor, discards invalid echoes, sorts valid samples, and returns the median in millimeters.
 
### `localData`
Manages an in-memory measurement buffer backed by LittleFS. Automatically persists measurements to flash when WiFi is unavailable (throttled to `FLASH_SAVE_INTERVAL`). Loads stored measurements on boot.
 
### `commands`
Polls `GET /devices/:id/commands` for pending commands and processes them:
 
| Command | Action |
|---------|--------|
| `set_interval` | Updates the measurement interval and saves to NVS |
| `update_wifi` | Saves new WiFi credentials and restarts |
| `restart` | Restarts the device |
| `remove_device` | Deletes the device from the server, clears all local data, enters setup mode |
 
Each command is acknowledged with `processing` before handling and `completed` or `failed` after.
 
### `setupMode`
Starts a WiFi access point and runs a lightweight HTTP server on port 80. Accepts device configuration via `POST /setup`. Times out after `SETUP_TIMEOUT_MS` and restarts if no configuration is received.
 
### `ota`
Compares the locally saved firmware version against the latest version from the API. Downloads the binary over HTTPS with TLS validation and installs it using the ESP32 `Update` library. Saves the new version to NVS after a successful install.
 
### `credentials`
Wraps ESP32 `Preferences` (NVS) for persistent storage of device ID, secret, WiFi credentials, provisioning status, and measurement interval.
 
---
 
## Author
 
**Mattias Mellin**\
<mm225vh@student.lnu.se> · <mattias.mellin@gmail.com>\
[GitHub](https://github.com/M-Mellin) · [LinkedIn](https://www.linkedin.com/in/mattias-mellin-22a283267/)
 