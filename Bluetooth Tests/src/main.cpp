#include <BleKeyboard.h>
#include <NimBLEDevice.h>
#include <NimBLEAdvertisedDevice.h>
#include "NimBLEEddystoneTLM.h"
#include "NimBLEBeacon.h"

// Set the BLE device name and manufacturer
BleKeyboard bleKeyboard("solid watch", "MyCompany", 100);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE keyboard with NimBLE...");
  delay(1000);

  // Initialize NimBLE (GAP device name stored in GATT)
  NimBLEDevice::setSecurityAuth(true, true, true);  // bonding, MITM, secure
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);

  // Start the HID server (this registers services/characteristics)
  bleKeyboard.begin();
  delay(500);

  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->setName("solid watch");                     // complete local name
  pAdvertising->addServiceUUID(NimBLEUUID((uint16_t)0x1812)); // HID service
  NimBLEDevice::startAdvertising();
  delay(200);
}

void loop() {
  // Wait for BLE client to connect
  if (bleKeyboard.isConnected()) {
    delay(10000);
    Serial.println("Client connected!");
    Serial.println("Sending Enter key...");
    bleKeyboard.write(KEY_RETURN);
    delay(5000);
    Serial.println("Sending Play/Pause media key...");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    
  } else {
    NimBLEDevice::stopAdvertising();
    NimBLEDevice::startAdvertising();
    delay(1000);
  }

}









