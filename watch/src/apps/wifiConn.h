#pragma once

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "time.h"

// Forward declaration of printStrings and flexRead from main.cpp
void printStrings(String str, int size, int x, int y);
void flexRead();

class WifiConn {
public:
    WifiConn(Adafruit_SSD1306* disp);  // Pass pointer to OLED
    void start();   // Called once to connect
    void connecting();  
    void setBendStates(int b1, int b2);

private:
    Adafruit_SSD1306* display;
    const char* ssid = "Nothing Phone";
    const char* password = "password";
    const char* ntpServer = "pool.ntp.org";
    const long gmtOffset_sec = 0;
    const int daylightOffset_sec = 3600;
    bool turnOn = true;
    bool select = false;
    int cords[2] = {0,0};
    int bent1;
    int bent2;
    int t = 0;
};