#include <Arduino.h>
#include <Wire.h>

void setup() {
  // Use Keyestudio ESP32 Plus hardware I2C pins
  Wire.begin(21, 22);
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\n--- I2C Scanner Ready ---");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning for I2C devices...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Found I2C device at address: 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    }
  }
 
  if (nDevices == 0) {
    Serial.println("No I2C devices found. Check your wiring!\n");
  } else {
    Serial.println("Scan complete.\n");
  }

  delay(3000); // Scan every 3 seconds
}