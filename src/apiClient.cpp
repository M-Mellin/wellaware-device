#include "apiClient.h"

ApiClient::ApiClient() {
  _client.setCACert(ROOT_CA_CERT);
}

void ApiClient::begin(HTTPClient& http, const String& url, const String& token) {
  http.setTimeout(API_TIMEOUT_MS);
  http.setReuse(false);
  http.begin(_client, url);
  http.addHeader("Content-Type", "application/json");
  if (token.length() > 0) {
    http.addHeader("Authorization", "Bearer " + token);
  }
}

ApiResponse ApiClient::get(const String& url, const String& token) {
  HTTPClient http;
  begin(http, url, token);
  ApiResponse res;
  res.code = http.GET();
  res.body = http.getString();
  http.end();
  return res;
}

ApiResponse ApiClient::post(const String& url, const String& body, const String& token) {
  HTTPClient http;
  begin(http, url, token);
  ApiResponse res;
  res.code = http.POST(body);
  res.body = http.getString();
  http.end();
  return res;
}

ApiResponse ApiClient::patch(const String& url, const String& body, const String& token) {
  HTTPClient http;
  begin(http, url, token);
  ApiResponse res;
  res.code = http.PATCH(body);
  res.body = http.getString();
  http.end();
  return res;
}

ApiResponse ApiClient::del(const String& url, const String& token) {
  HTTPClient http;
  begin(http, url, token);
  ApiResponse res;
  res.code = http.sendRequest("DELETE");
  res.body = http.getString();
  http.end();
  return res;
}