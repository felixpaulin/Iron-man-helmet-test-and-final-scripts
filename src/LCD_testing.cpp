#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

static const uint8_t LCD_ADDRESS = 0x27;
static const uint8_t I2C_SDA_PIN = 21;
static const uint8_t I2C_SCL_PIN = 22;
static const char spinnerChars[] = {'|', '/', '-', '\\'};

LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);
unsigned long lastSpinnerUpdate = 0;
uint8_t spinnerIndex = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Hello, World!");

  lcd.setCursor(0, 1);
  lcd.print("LCD works");

  lcd.setCursor(15, 1);
  lcd.print(spinnerChars[spinnerIndex]);

  Serial.println("LCD ready on SDA 21 / SCL 22");
}

void loop() {
  unsigned long now = millis();

  if (now - lastSpinnerUpdate >= 200) {
    lastSpinnerUpdate = now;
    spinnerIndex = (spinnerIndex + 1) % 4;

    lcd.setCursor(15, 1);
    lcd.print(spinnerChars[spinnerIndex]);
  }
}
