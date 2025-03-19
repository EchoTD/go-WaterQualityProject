#include <Arduino.h>
#include "SensorManager.h"
#include "Data.h"
#include "HTTPManager.h"
#include <WiFiManager.h>
#include <Preferences.h>

Preferences preferences;
WiFiManager wifiManager;
SensorManager sensorManager;
HTTPManager httpManager;

const unsigned long averagingInterval = 5000;
const unsigned long sendingInterval = 5000;
const int maxReadings = 10;

float temperatureReadings[maxReadings];
int readingIndex = 0;
unsigned long lastAveragingTime = 0;
unsigned long lastSendingTime = 0;

/* const char* ssid = "TurkTelekom_TP5000_2.4GHz";
const char* password = "X3PHepLwN9Jc";
const char* serverName = "http://192.168.1.111:8080"; */

char ssid[32] = "";
char password[32] = "";

void setup() {
  Serial.begin(115200);
  sensorManager.begin();

  loadCredentials(ssid, password);

  if (strlen(ssid) == 0 || strlen(password) == 0) {
      // No credentials found, start configuration mode
      if (!wifiManager.autoConnect("ESP32-Config")) {
          Serial.println("Failed to connect and hit timeout");
          delay(3000);
          ESP.restart();
      }
      saveCredentials(wifiManager.getSSID().c_str(), wifiManager.getPassword().c_str());
  } else {
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
      }
      Serial.println("Connected to WiFi");
      Serial.println(WiFi.localIP());
  }

  /* // wifiManager.resetSettings();

  if (!wifiManager.autoConnect("ESP32-Config")) {
      Serial.println("Failed to connect and hit timeout");
      delay(3000);
      ESP.restart();
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  //httpManager.setup(ssid, password); */

  for (int i = 0; i < maxReadings; i++) {
    temperatureReadings[i] = 0.0f;
  }

  Serial.println("System Initialized.");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastAveragingTime >= averagingInterval / maxReadings) {
    sensorManager.update();
    SensorData data = sensorManager.getSensorData();

    temperatureReadings[readingIndex] = data.temperature;
    readingIndex = (readingIndex + 1) % maxReadings;  // Circular buffer logic

    lastAveragingTime = currentTime;
  }

  if (currentTime - lastSendingTime >= sendingInterval) {
    float sum = 0.0f;
    int count = 0;

    for (int i = 0; i < maxReadings; i++) {
      if (temperatureReadings[i] != 0.0f) {
        sum += temperatureReadings[i];
        count++;
      }
    }

    if (count > 0) {
      float averageTemperature = sum / count;

      Serial.print(">avg_temp_c:");
      Serial.println(averageTemperature);

      httpManager.echoPacket(serverName, averageTemperature);

      for (int i = 0; i < maxReadings; i++) {
        temperatureReadings[i] = 0.0f;
      }
      readingIndex = 0;
    }

    lastSendingTime = currentTime;
  }
}

void saveCredentials(const char* ssid, const char* password) {
    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();
}

void loadCredentials(char* ssid, char* password) {
    preferences.begin("wifi", false);
    strcpy(ssid, preferences.getString("ssid", "").c_str());
    strcpy(password, preferences.getString("password", "").c_str());
    preferences.end();
}