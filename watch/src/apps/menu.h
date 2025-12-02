#pragma once 

#include "wifiConn.h"

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "time.h"

void printStrings(String str, int size, int x, int y);
extern void flexRead();

class Menu {
public:
    Menu(Adafruit_SSD1306* disp, WifiConn* wifiPtr);  

    void start(); 
    void setBendStates(int b1, int b2);

private:
    Adafruit_SSD1306* display;
    WifiConn* wifi;
    int bent1;
    int bent2;

    int l;
    bool select;
    int cords = 0;
    String apps[6] = {".settings", ".maps", ".timers", ".little friend", ".small balatro", ".randomizers"};
};