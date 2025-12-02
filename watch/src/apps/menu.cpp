#include "wifiConn.h"
#include "menu.h"
#include "img.h"

#include "time.h"
#include <Arduino.h>

// prototype for function implemented in main.cpp
extern void printStrings(String str, int size, int x, int y);

Menu::Menu(Adafruit_SSD1306* disp, WifiConn* wifiPtr){display = disp; wifi = wifiPtr;}

void Menu::setBendStates(int b1, int b2) {
    bent1 = b1;
    bent2 = b2;
}

void Menu::start(){
    select = false;

    display->clearDisplay();
    for (l = 0; l < 6; ) {
            printStrings(Menu::apps[l], 1, 10,10*l);
            display->drawBitmap(0, l*apps_height, apps_allArray[l], apps_width, apps_height, WHITE);
            l++;
        }
    display->display();
    
    delay(500);

    while (select == false){
        for (l = 0; l < 6; ) {
            display->fillRect(11,(l*10)+8,128,1,BLACK);
            l++;
        }

        flexRead();
        
        if (bent1==1 && bent2==1){
            select = true; 
        }
        else if (bent1==1 && bent2==0){
            if (cords < 5){
                cords++;
            }
            else{cords = 0;}
        }

        display->fillRect(10, cords*10+8, Menu::apps[cords].length()*6, 1, WHITE);

        display->display();

        delay(100);
    }

    display->clearDisplay();

    if (cords == 0){
        wifi->start();
    }
}