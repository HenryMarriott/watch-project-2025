#pragma once 

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "time.h"

void printStrings(String str, int size, int x, int y);

class watchface {
public:
    watchface(Adafruit_SSD1306* disp);  

    void start(); 
    void setBendStates(int b1, int b2);
    void setFirstUpdate(bool v);

private:
    Adafruit_SSD1306* display;
    int t = 0;
    struct tm prevTime;
    int bent1;
    int bent2;
};