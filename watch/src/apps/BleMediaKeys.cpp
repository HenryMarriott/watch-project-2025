#include "BleMediaKeys.h"
#include <NimBLEServer.h>
#include <string>

// Global pointer for BLE server callback
static BleMediaKeys* gMediaKeys = nullptr;

class MediaKeysServerCallbacks : public NimBLEServerCallbacks {
public:
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
        if (gMediaKeys) {
            gMediaKeys->onConnect();
        }
    }

    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
        if (gMediaKeys) {
            gMediaKeys->onDisconnect();
        }
    }
};

BleMediaKeys::BleMediaKeys(const char* deviceName, const char* manufacturer)
    : hid(nullptr), mediaKeysChar(nullptr), connected(false), 
      deviceName(deviceName), manufacturer(manufacturer) {
    gMediaKeys = this;
}

void BleMediaKeys::begin() {
    if (hid) return; // already initialized

    Serial.println("Initializing BLE Media Keys...");
    
    // Initialize BLE device
    NimBLEDevice::init(deviceName);
    NimBLEDevice::setSecurityAuth(false, false, false);
    
    // Create BLE server
    NimBLEServer* pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new MediaKeysServerCallbacks());
    
    // Create HID device
    hid = new NimBLEHIDDevice(pServer);
    
    // Set minimal HID report map for Consumer Control (media keys)
    // Report ID 1: Consumer Control
    uint8_t reportMap[] = {
        0x05, 0x0C,        // Usage Page (Consumer Devices)
        0x09, 0x01,        // Usage (Consumer Control)
        0xA1, 0x01,        // Collection (Application)
        0x85, 0x01,        //   Report ID (1)
        0x19, 0x00,        //   Usage Minimum (Unassigned)
        0x2A, 0xFF, 0x03,  //   Usage Maximum (Note: this is a 16-bit value in little endian)
        0x15, 0x00,        //   Logical Minimum (0)
        0x26, 0xFF, 0x03,  //   Logical Maximum (1023)
        0x95, 0x01,        //   Report Count (1)
        0x75, 0x10,        //   Report Size (16)
        0x81, 0x00,        //   Input (Data, Array)
        0xC0               // End Collection
    };
    
    hid->setReportMap(reportMap, sizeof(reportMap));
    hid->startServices();
    
    // Get media keys characteristic (report ID 1)
    mediaKeysChar = hid->getInputReport(1);
    
    // Start advertising
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(hid->getHidService()->getUUID());
    NimBLEDevice::startAdvertising();
    
    Serial.println("BLE Media Keys initialized and advertising");
}

void BleMediaKeys::end() {
    if (hid) {
        NimBLEDevice::stopAdvertising();
        NimBLEDevice::deinit(true);
        hid = nullptr;
        mediaKeysChar = nullptr;
        connected = false;
    }
    Serial.println("BLE Media Keys stopped");
}

void BleMediaKeys::pressMediaKey(MediaKey key) {
    if (!mediaKeysChar || !connected) {
        return;
    }
    
    uint8_t report[2] = {(uint8_t)key, 0};
    mediaKeysChar->setValue(report, sizeof(report));
    mediaKeysChar->notify();
}

void BleMediaKeys::releaseMediaKey() {
    if (!mediaKeysChar || !connected) {
        return;
    }
    
    uint8_t report[2] = {0, 0};
    mediaKeysChar->setValue(report, sizeof(report));
    mediaKeysChar->notify();
}

void BleMediaKeys::sendMediaKey(MediaKey key) {
    pressMediaKey(key);
    delay(50);
    releaseMediaKey();
}

void BleMediaKeys::setBatteryLevel(uint8_t level) {
    // Battery level setting not implemented in minimal version
    (void)level;
}

void BleMediaKeys::onConnect() {
    connected = true;
    Serial.println("BLE Media Keys: Connected");
}

void BleMediaKeys::onDisconnect() {
    connected = false;
    Serial.println("BLE Media Keys: Disconnected");
}
