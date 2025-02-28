#ifndef HTTP_H
#define HTTP_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Data.h>



#define serverName = "http://192.168.1.111"

class HTTP;

class HTTP {
public:
    HTTP();
    void begin(char* ssid, char* password);

private:

    void readTemperature();

};

#endif