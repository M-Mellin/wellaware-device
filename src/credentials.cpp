#include <Preferences.h>

Preferences prefs;

void saveCredentials(String id, String secret, bool provisionStatus) {
  prefs.begin("device", false);
  prefs.putString("id", id);
  prefs.putString("secret", secret);
  prefs.putBool("provisioned", provisionStatus);
  prefs.end();
}

void saveInterval(int interval) {
  prefs.begin("device", false);
  prefs.putInt("interval", interval);
  prefs.end();
}

void loadInterval(int &interval) {
  prefs.begin("device", true);

  interval = prefs.getInt("interval", 10000);
  prefs.end();
}

bool loadCredentials(String &id, String &secret, bool &provisionStatus) {
  prefs.begin("device", true);

  id = prefs.getString("id", "");
  secret = prefs.getString("secret", "");
  provisionStatus = prefs.getBool("provisioned", false);

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

void clearAllData() {
  prefs.begin("device", false);
  prefs.clear();
  prefs.end();

  prefs.begin("wifi", false);
  prefs.clear();
  prefs.end();

  prefs.begin("interval", false);
  prefs.clear();
  prefs.end();
}



