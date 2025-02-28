#include <Arduino.h>
#include "SensorManager.h"
#include "Data.h"

SensorManager sensorManager;

void setup() {
    Serial.begin(115200);
    sensorManager.begin();

    Serial.println("System Initialized.");
}

void loop() {
    sensorManager.update();
    SensorData data = sensorManager.getSensorData();

    Serial.print(">temp_c:");
    Serial.println(data.temperature);
}