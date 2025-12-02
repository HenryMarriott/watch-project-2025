#include "wifiConn.h"
#include <WiFi.h>
#include "time.h"
#include <Arduino.h>

// prototype for function implemented in main.cpp
extern void printStrings(String str, int size, int x, int y);

WifiConn::WifiConn(Adafruit_SSD1306* disp) {
    display = disp;
}
void WifiConn::setBendStates(int b1, int b2) {
    bent1 = b1;
    bent2 = b2;
}


void WifiConn::start() {
    if (turnOn == true) {
        WifiConn::connecting();
    }
    
    else{
        select = false;

        display->clearDisplay();
        printStrings("Time Reset:  go", 1, 0,0);
        printStrings("Bluetooth:   on/off", 1, 0,10);
        printStrings("exit?", 1, 5,20);

        delay(500);

        
        while (select == false){
            flexRead();
            // check for select input (2 bent fingers)
            if (bent1==1 && bent2==1){
                select = true;
            }
            
            if (bent1==1 && bent2 == 0){
                if (cords[1] < 2){
                    cords[1]++;
                }
                else {cords[1]=0;}
            }
            if (bent2==1 && bent1 == 0){
                if (cords[0] < 1){
                    cords[0]++;
                }
                else {cords[0]=0;}
            }
            
            display->fillRect(0,8,128,1,BLACK);
            display->fillRect(0,18,128,1,BLACK);
            display->fillRect(0,28,128,1,BLACK);
            if (cords[1]==0){
                display->fillRect((6*13),8,(6*2),1,WHITE);
            }
            else if (cords[1]==1){
                if (cords[0]==0){
                    display->fillRect((6*13),18,(6*2),1,WHITE);
                }
                else{
                    display->fillRect((6*16),18,(6*3),1,WHITE);
                }
            }
            else if (cords[1]==2){
                display->fillRect(5,28,(6*4),1,WHITE);
            }
            // check for a change in co-oridinates 
            // display underline depending on co-ordinates 
            display->display();
            delay(200);
        }
        if (cords[1]==0){
            WifiConn::connecting();
        }
        display->clearDisplay();
    }
    turnOn = false;
}

void WifiConn::connecting(){
    display->clearDisplay();
    int w = 0;
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        Serial.print("Connecting... Status: ");
        Serial.println(WiFi.status());

        display->clearDisplay();
        if (w == 0){ printStrings("_", 1, 0,0); w++; }
        else { printStrings(" ", 1, 0,0); w=0; }
        display->display();
    }

    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(WHITE);

    printStrings("i_", 1, 0,0);
    display->display();
    delay(25);
    display->fillRect(6,0,6,8,BLACK);
    printStrings("i'_", 1, 0,0);
    display->display();
    delay(25);
    display->fillRect(12,0,6,8,BLACK);
    printStrings("i'm_", 1, 0,0);
    display->display();
    delay(25);
    display->fillRect(18,0,6,8,BLACK);
    printStrings("i'm _", 1, 0,0);
    display->display();
    delay(25);
    display->fillRect(24,0,6,8,BLACK);
    printStrings("i'm i_", 1, 0,0);
    display->display();
    delay(25);
    display->fillRect(30,0,6,8,BLACK);
    printStrings("i'm in_", 1, 0,0);
    display->display();
    delay(25);
    display->fillRect(36,0,6,8,BLACK);
    printStrings("i'm in._", 1, 0,0);
    display->display();

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
        Serial.println("Waiting for NTP time...");
        delay(100);
    }

    WiFi.disconnect(true);
    turnOn = false;

    display->clearDisplay();
}