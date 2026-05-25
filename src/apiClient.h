#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "../config.h"

struct ApiResponse {
  int code;
  String body;
};

class ApiClient {
public:
  ApiClient();
  ApiResponse get(const String& url, const String& token = "");
  ApiResponse post(const String& url, const String& body, const String& token = "");
  ApiResponse patch(const String& url, const String& body, const String& token = "");
  ApiResponse del(const String& url, const String& token = "");

private:
  WiFiClientSecure _client;
  void begin(HTTPClient& http, const String& url, const String& token);
};