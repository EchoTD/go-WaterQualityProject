#include <Arduino.h>
#include "SensorManager.h"
#include "MQTTManager.h"
#include <WiFiManager.h>
#include <Preferences.h>

/*---CONFIG---*/
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

const unsigned long SENSOR_READ_INTERVAL = 500;
const unsigned long MQTT_SEND_INTERVAL = 5000;
const int MAX_READINGS = 10;

float tempReadings[MAX_READINGS] = {0};
//float tdsReadings[MAX_READINGS] = {0};

int readingIndex = 0;
unsigned long lastSensorReadTime = 0;
unsigned long lastMqttSendTime = 0;

Preferences preferences;
SensorManager sensorManager;
MQTTManager mqttManager(mqttServer, mqttPort);

void readSensorData();
void sendAveragedData();
float calculateAverage(float* buffer, int size);
void resetReadings();

void setup() {
  Serial.begin(115200);
  sensorManager.begin();
  mqttManager.begin();
  Serial.println("System Initialized.");
}

void loop() {
  mqttManager.loop();

  Serial.println(WiFi.localIP());
  
  readSensorData();
  sendAveragedData();
}

void readSensorData() {
  if (millis() - lastSensorReadTime >= SENSOR_READ_INTERVAL) {
    sensorManager.update();
    SensorData data = sensorManager.getSensorData();
    
    tempReadings[readingIndex] = data.temperature;
    //tdsReadings[readingIndex] = data.tds;
    
    readingIndex = (readingIndex + 1) % MAX_READINGS;
    lastSensorReadTime = millis();
  }
}

void sendAveragedData() {
  if (millis() - lastMqttSendTime >= MQTT_SEND_INTERVAL) {
    float avgTemp = calculateAverage(tempReadings, MAX_READINGS);
    //float avgTds = calculateAverage(tdsReadings, MAX_READINGS);

    // Send data via MQTT
    if (!isnan(avgTemp))      mqttManager.sendData("temperature", &avgTemp, 1);
    //if (!isnan(avgTds))       mqttManager.sendData("tds", &avgTds, 1);

    resetReadings();
    lastMqttSendTime = millis();
  }
}

float calculateAverage(float* buffer, int size) {
  float sum = 0;
  int validReadings = 0;
  for (int i = 0; i < size; i++) {
    if (!isnan(buffer[i])) {
      sum += buffer[i];
      validReadings++;
    }
  }
  return (validReadings > 0) ? sum / validReadings : NAN;
}

void resetReadings() {
  memset(tempReadings, 0, sizeof(tempReadings));
  //memset(tdsReadings, 0, sizeof(tdsReadings));
  readingIndex = 0;
}