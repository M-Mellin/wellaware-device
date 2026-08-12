/**
 * @file apiClient.cpp
 * @brief HTTPS client implementation for communicating with the WellAware API.
 *
 * @author Mattias Mellin
 * @email mm225vh@student.lnu.se | mattias.mellin@gmail.com
 */

#include "apiClient.h"

/**
 * @brief Initializes the ApiClient with a valid TLS certificate.
 * The certificate is loaded from ROOT_CA_CERT defined in config.h.
 */
ApiClient::ApiClient() {
  _client.setCACert(ROOT_CA_CERT);
}

/**
 * @brief Configures and opens a secure HTTPS connection.
 *
 * @param http   Reference to the HTTPClient instance to configure.
 * @param url    Full URL to connect to.
 * @param token  JWT token for authentication. Omitted if empty string.
 */
void ApiClient::begin(HTTPClient& http, const String& url, const String& token) {
  _client.stop();
  delay(100);

  http.setTimeout(API_TIMEOUT_MS);
  http.setReuse(false);
  http.begin(_client, url);

  http.addHeader("Content-Type", "application/json");

  if (token.length() > 0) {
    http.addHeader("Authorization", "Bearer " + token);
  }
}

/**
 * @brief Sends an HTTP GET request.
 *
 * @param url    Full URL to fetch from.
 * @param token  JWT token for authentication. Optional.
 * @return ApiResponse containing the HTTP status code and response body.
 */
ApiResponse ApiClient::get(const String& url, const String& token) {
  HTTPClient http;
  begin(http, url, token);
  ApiResponse res;
  res.code = http.GET();
  res.body = http.getString();
  http.end();
  return res;
}

/**
 * @brief Sends an HTTP POST request with a JSON body.
 *
 * @param url    Full URL to post to.
 * @param body   JSON string to send as the request body.
 * @param token  JWT token for authentication. Optional.
 * @return ApiResponse containing the HTTP status code and response body.
 */
ApiResponse ApiClient::post(const String& url, const String& body, const String& token) {
  HTTPClient http;
  begin(http, url, token);
  ApiResponse res;
  res.code = http.POST(body);
  res.body = http.getString();
  http.end();
  return res;
}

/**
 * @brief Sends an HTTP PATCH request with a JSON body.
 *
 * @param url    Full URL to patch.
 * @param body   JSON string to send as the request body.
 * @param token  JWT token for authentication. Optional.
 * @return ApiResponse containing the HTTP status code and response body.
 */
ApiResponse ApiClient::patch(const String& url, const String& body, const String& token) {
  HTTPClient http;
  begin(http, url, token);
  ApiResponse res;
  res.code = http.PATCH(body);
  res.body = http.getString();
  http.end();
  return res;
}

/**
 * @brief Sends an HTTP DELETE request.
 *
 * @param url    Full URL to delete.
 * @param token  JWT token for authentication. Optional.
 * @return ApiResponse containing the HTTP status code and response body.
 */
ApiResponse ApiClient::del(const String& url, const String& token) {
  HTTPClient http;
  begin(http, url, token);
  ApiResponse res;
  res.code = http.sendRequest("DELETE");
  res.body = http.getString();
  http.end();
  return res;
}