#include "apps/wifiConn.h"
#include "apps/watchface.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//font size 1 = 6x8 pixels
#include <WiFi.h>
#include "time.h"
#include <BleKeyboard.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Reset pin (not used)
#define SCREEN_ADDRESS 0x3C // Common I2C address for OLED displays

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WifiConn wifi(&display);
watchface wf(&display);


int t = 0;

const int flex1 = A2;  // GPIO 4 (analog capable)
int bent1 = 0;
const int flex2 = A1;  // GPIO 5 (analog capable)
int bent2 = 0;
bool select1;
int cords = 0;
int l;
String apps[3] = {".settings", ".maps", ".timers"};
int buttConn;

void printStrings(String str, int size, int x, int y);
void flexRead();

void setup() {
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("SSD1306 not detected! Check wiring.");
        while (true);
    }

    wifi.start();
}

void loop() {
    flexRead();
    wf.setBendStates(bent1, bent2);
    wifi.setBendStates(bent1, bent2);
    if (t > 9){
        if (wifi.isBleConnected()) {wf.conn();}
        else {wf.disconn();}
        t = 0;
    } else {t++;}
    if (bent1 == 1 or bent2 == 1) {
        buttConn++;
        if (buttConn == 30){
            buttConn = 0;
            if (bent1 == 1 && bent2 == 1){
                select1 = false;
                display.clearDisplay();
                for (l = 0; l < 3; ) {
                        printStrings(apps[l], 1, 10,10+10*l);
                        l++;
                    }
                display.display();    
                delay(500);
                while (select1 == false){
                    for (l = 0; l < 3; ) {
                        display.fillRect(10,10+(l*10)+8,128,1,BLACK);
                        l++;
                    }
                    flexRead();
                    if (bent1==1 && bent2==1){
                        select1 = true; 
                    }
                    else if (bent1==1 && bent2==0){
                        if (cords < 2){
                            cords++;
                        }
                        else{cords = 0;}
                    }
                    display.fillRect(10,10+(cords*10+8), apps[cords].length()*6, 1, WHITE);
                    wf.menu();
                    display.display();
                    delay(200);
                }
                display.clearDisplay();

                if (cords == 0){
                    wifi.start();
                }
            }
            else if (wifi.isBleConnected()) {
                if (bent1 == 1 && bent2 == 0){
                    wifi.pause();
                }
                else if (bent1 == 0 && bent2 == 1){
                    wifi.skip();
                }
            }
        }
    else {}
    }
    delay(10);

}

void printStrings(String str, int size, int x, int y){
    char space = ' ';
    int length = str.length();
    int max = 128/(6*size);

    if (length > max){
        while (str[max] != space){max--;}
        String line1 = String(str).substring(0,max);
        String line2 = String(str).substring(max+1,length);
        display.setTextSize(size);
        display.setTextColor(WHITE);
        display.setCursor(x,y);
        display.print(line1);
        display.setCursor(x,y+(8*size)+1);
        display.print(line2);
    } else {
        display.setTextSize(size);
        display.setTextColor(WHITE);
        display.setCursor(x,y);
        display.print(str);
    }
}

void flexRead() {
    int flex1An = analogRead(flex1);
    if (flex1An > 3000){bent1=1;}
    else {bent1=0;} 
    int flex2An = analogRead(flex2);
    if (flex2An < 50){bent2=1;}
    else {bent2=0;}

    Serial.print(flex1An); Serial.print("  "); 
    Serial.println(flex2An);

    wf.setBendStates(bent1, bent2);
    wifi.setBendStates(bent1, bent2);
}
