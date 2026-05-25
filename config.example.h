#pragma once

// Server
#define API_BASE_URL "https://mellin.net/wellaware/api/v1"
#define API_TIMEOUT_MS 5000

// Setup mode
#define SETUP_AP_SSID "WellAware-Device-Setup"
#define SETUP_TIMEOUT_MS 30000

// Retrived with: openssl s_client -connect dittdomän.se:443 </dev/null 2>/dev/null | openssl x509 -outform PEM
static const char ROOT_CA_CERT[] = R"(
-----BEGIN CERTIFICATE-----
CERTIFIKAT
-----END CERTIFICATE-----
)";