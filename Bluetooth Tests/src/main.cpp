#include <BleKeyboard.h>

// Set the BLE device name and manufacturer
BleKeyboard bleKeyboard("C6 BLE Keyboard", "MyCompany", 100);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE keyboard with NimBLE...");

  // Give the ESP32 some time to initialize BLE stack
  delay(2000);

  // Begin advertising
  bleKeyboard.begin();

  Serial.println("BLE keyboard advertising started!");
}

void loop() {
  // Wait for BLE client to connect
  if (bleKeyboard.isConnected()) {
    Serial.println("Client connected!");

    Serial.println("Sending Enter key...");
    bleKeyboard.write(KEY_RETURN);
    delay(5000);

    Serial.println("Sending Play/Pause media key...");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    delay(5000);

    // Optional: add more key sequences here

    delay(10000);  // Wait 5 seconds before repeating
  } else {
    // No client connected
    Serial.println("Waiting for client...");
    delay(1000);
  }
}













