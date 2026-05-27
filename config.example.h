#pragma once

// Server
#define API_BASE_URL "https://your-domain.net"
#define API_TIMEOUT_MS 5000

// Setup Mode
#define SETUP_AP_SSID "WellAware-Device-Setup"
#define SETUP_TIMEOUT_MS 30000

// Local Data
#define FLASH_SAVE_INTERVAL 15 * 60 * 1000
#define MEASUREMENTS_FILE "/measurements.bin"
#define MAX_MEASUREMENTS 500

// Ultrasonic setup
#define ECHO_PIN 4
#define TRIG_PIN 5
#define MEDIAN_SAMPLES 12

// Calculation Method
#define DEFAULT_CALCULATION_METHOD "validation"
#define CALCULATION_METHOD_FILE "/calculation_method.txt"
#define VALIDATION_MIN_INTERVAL_SEC 60

// Token refresh interval
#define TOKEN_REFRESH_INTERVAL 3600000

// Over-The-Air check for updates interval
#define OTA_CHECK_INTERVAL 6UL * 60UL * 60UL * 1000UL

// NTP Server polling
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 0
#define DAYLIGHT_OFFSET_SEC 0

// Server Certificate
static const char ROOT_CA_CERT[] = R"(
-----BEGIN CERTIFICATE-----
CERTIFICATE
-----END CERTIFICATE-----
)";