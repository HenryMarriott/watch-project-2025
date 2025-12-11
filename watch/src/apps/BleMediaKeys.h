#pragma once

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>

/**
 * Minimal BLE HID Media Keys Controller
 * Stripped-down version of BleKeyboard for media keys only (play, pause, volume, etc)
 * Estimated size: ~50-80 KB (vs 400 KB for full BleKeyboard)
 */
class BleMediaKeys {
public:
    // Media key codes (Consumer Control Usage IDs)
    enum MediaKey : uint8_t {
        KEY_MEDIA_PLAY_PAUSE = 0xCD,
        KEY_MEDIA_NEXT_TRACK = 0xB5,
        KEY_MEDIA_PREVIOUS_TRACK = 0xB6,
        KEY_MEDIA_STOP = 0xB7,
        KEY_MEDIA_MUTE = 0xE2,
        KEY_MEDIA_VOLUME_UP = 0xE9,
        KEY_MEDIA_VOLUME_DOWN = 0xEA,
    };

    BleMediaKeys(const char* deviceName = "ESP32-Media", const char* manufacturer = "Espressif");
    
    void begin();
    void end();
    
    // Send media key press and release
    void pressMediaKey(MediaKey key);
    void releaseMediaKey();
    void sendMediaKey(MediaKey key);  // press + release
    
    bool isConnected() const { return connected; }
    void setBatteryLevel(uint8_t level);
    
    // Called by callback helper
    void onConnect();
    void onDisconnect();

private:
    NimBLEHIDDevice* hid;
    NimBLECharacteristic* mediaKeysChar;
    bool connected;
    const char* deviceName;
    const char* manufacturer;
};

