#if __has_include(<Arduino.h>)
#include <Arduino.h>
#else
// Fallback for editors/IntelliSense on non-Arduino hosts
#include <cstdint>
// Use standard fixed-width integer types from <cstdint>
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using byte = std::uint8_t;
inline void delay(unsigned long) {}
#endif

#if __has_include(<Wire.h>)
#include <Wire.h>
#else
// Minimal Wire stub for non-Arduino environment
struct TwoWire {
  void begin(int, int) {}
} Wire;
#endif

#if __has_include(<LiquidCrystal_I2C.h>)
#include <LiquidCrystal_I2C.h>
#else
// Minimal LiquidCrystal_I2C stub to allow compilation in host editors
class LiquidCrystal_I2C {
public:
  LiquidCrystal_I2C(int, int, int) {}
  LiquidCrystal_I2C(int, int) {}
  void init() {}
  void backlight() {}
  void setCursor(int, int) {}
  void print(const char*) {}
};
#endif

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize the LCD with the I2C address and dimensions

void setup() {

    Wire.begin(21, 22);  // Initialize I2C communication

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