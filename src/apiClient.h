/**
 * @file apiClient.h
 * @brief HTTPS client for communicating with the WellAware API.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "../config.h"

/**
 * @brief Holds the result of an HTTP request.
 */
struct ApiResponse {
  int code;
  String body;
};

/**
 * @brief HTTPS client for communicating with the WellAware API.
 *
 * Wraps WiFiClientSecure and HTTPClient to provide a simple
 * interface for GET, POST, PATCH and DELETE requests with
 * TLS certificate validation and JWT authentication.
 */

class ApiClient {
public:
  /**
   * @brief Initializes the client with the TLS root CA certificate.
   */
  ApiClient();
  ApiResponse get(const String& url, const String& token = "");
  ApiResponse post(const String& url, const String& body, const String& token = "");
  ApiResponse patch(const String& url, const String& body, const String& token = "");
  ApiResponse del(const String& url, const String& token = "");

private:
  WiFiClientSecure _client;
  
    /**
   * @brief Configures and opens a secure HTTPS connection.
   *
   * @param http   Reference to the HTTPClient instance to configure.
   * @param url    Full URL to connect to.
   * @param token  JWT token for authentication. Omitted if empty string.
   */
  void begin(HTTPClient& http, const String& url, const String& token);
};