#include "HTTP.h"
#include "Data.h"

HTTP::HTTP(): {}

void HTTP::begin(ssid*, password*) {
    WiFi.begin(ssid, password);
    Serial.println("Connecting...");
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("#");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}