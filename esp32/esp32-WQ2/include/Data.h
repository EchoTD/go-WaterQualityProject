#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

static const uint8_t TEMP_SENSOR_PIN = 4;

typedef struct {
    char ssid[] = "TurkTelekom_TP5000_2.4GHz";
    char password[] = "X3PHepLwN9Jc";
    float temperature;      // in °C 
} SensorData;

#endif