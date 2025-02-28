#include <Arduino.h>
#include "SensorManager.h"
#include "Data.h"
#include "HTTPManager.h"

SensorManager sensorManager;
HTTPManager httpManager;

unsigned long lastTime = 0;
unsigned long timeDelay = 1000;

const char* ssid = "TurkTelekom_TP5000_2.4GHz";
const char* password = "X3PHepLwN9Jc";
const char* serverName = "http://192.168.1.111";

void setup() {
  Serial.begin(115200);
  sensorManager.begin();

  httpManager.setup(ssid, password);

  Serial.println("System Initialized.");
}

void loop() {
  sensorManager.update();
  SensorData data = sensorManager.getSensorData();

  Serial.print(">temp_c:");
  Serial.println(data.temperature);
  
  if((millis() - lastTime) > timeDelay) {
    httpManager.echoPacket(serverName, data.temperature);
  }
}