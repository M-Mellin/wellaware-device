#include <Preferences.h>

Preferences prefs;
void saveCredentials(String id, String secret) {
  prefs.begin("device", false);
  prefs.putString("id", id);
  prefs.putString("secret", secret);
  prefs.end();
}

bool loadCredentials(String &id, String &secret) {
  prefs.begin("device", true);

  id = prefs.getString("id", "");
  secret = prefs.getString("secret", "");

  prefs.end();

  return id.length() > 0 && secret.length() > 0;
}

void saveWifi(String ssid, String password) {
  prefs.begin("wifi", false);
  prefs.putString("ssid", ssid);
  prefs.putString("password", password);
  prefs.end();
}

bool loadWifi(String &ssid, String &password) {
  prefs.begin("wifi", true);

  ssid = prefs.getString("ssid", "");
  password = prefs.getString("password", "");

  prefs.end();

  return ssid.length() > 0 && password.length() > 0;
}



