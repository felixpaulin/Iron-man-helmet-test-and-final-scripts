#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize the LCD with the I2C address and dimensions

void setup() {

    Wire.begin();  // Initialize I2C communication

  lcd.init();  // Initialize the LCD
  lcd.backlight();  // Turn on the backlight

lcd.setCursor(0, 0);  // Set the cursor to the first column of the first row
lcd.print("Hello, World!");  // Print a message on the LCD

lcd.setCursor(0, 1);  // Set the cursor to the first column of the second row
lcd.print("PlatformIO OK");  // Print another message on the LCD
}

void loop() {
  // You can add code here to update the LCD display if needed
}