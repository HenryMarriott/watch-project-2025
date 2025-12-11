#include "watchface.h"

#include "time.h"
#include <Arduino.h>

// prototype for function implemented in main.cpp
extern void printStrings(String str, int size, int x, int y);

watchface::watchface(Adafruit_SSD1306* disp) {
    display = disp;
}

void watchface::setBendStates(int b1, int b2) {
    bent1 = b1;
    bent2 = b2;
}


void watchface::start(){
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return;   
    }

    char dayStr[20];
    strftime(dayStr, sizeof(dayStr), "%A", &timeinfo);
    String dayA = String(dayStr).substring(0, 2);
    char monthStr[20];
    strftime(monthStr, sizeof(monthStr), "%B", &timeinfo);
    String monthA = String(monthStr).substring(0, 3);
    char yearStr[20];
    strftime(yearStr, sizeof(yearStr), "%Y", &timeinfo);
    String yearA = String(yearStr).substring(3, 5);

    display->clearDisplay();

    String bottomText = "kept you waiting, huh?";
    printStrings(bottomText, 1, 0, 48);
    display->setTextSize(1);
    display->setTextColor(WHITE);
    display->setCursor(0, 10);
    display->print(dayA);
    display->print(", ");
    display->print(&timeinfo, "%d");
    display->print(" ");
    display->println(monthA);

    display->setTextSize(3);
    display->setCursor(0, 18);
    display->print(&timeinfo, "%H:%M");
    display->setTextSize(2);
    display->setCursor(90,25);
    display->print(&timeinfo, "%S");
    display->display();

    display->fillRect(110, 0, 6, 8, BLACK);
    display->setTextSize(1);
    display->setTextColor(WHITE);
    display->setCursor(110, 0);
    display->print(bent1);
    display->fillRect(104, 0, 6, 8, BLACK);
    display->setTextSize(1);
    display->setTextColor(WHITE);
    display->setCursor(104, 0);
    display->print(bent2);

    display->display();

    prevTime = timeinfo;
}