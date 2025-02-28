#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

static const uint8_t TDS_PIN         = 34;
static const uint8_t TURBIDITY_PIN   = 35;
static const uint8_t FLOWMETER1_PIN  = 16;
static const uint8_t FLOWMETER2_PIN  = 17;
static const uint8_t TEMP_SENSOR_PIN = 4;

typedef struct {
    float temperature;      // in °C 
} SensorData;

#endif