#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

static const uint8_t LCD_ADDRESS = 0x27;
static const uint8_t I2C_SDA_PIN = 21;
static const uint8_t I2C_SCL_PIN = 22;
static const uint8_t BUTTON_PIN = 19;
static const uint8_t SERVO_PIN = 18;
static const uint8_t SERVO_PWM_CHANNEL = 0;
static const uint16_t SERVO_PWM_FREQUENCY = 50;
static const uint8_t SERVO_PWM_RESOLUTION = 16;
static const uint32_t SERVO_MIN_PULSE_US = 500;
static const uint32_t SERVO_MAX_PULSE_US = 2400;
static const char spinnerChars[] = {'|', '/', '-', '\\'};

LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);
unsigned long lastSpinnerUpdate = 0;
uint8_t spinnerIndex = 0;
bool helmetOpen = false;
int buttonStableState = HIGH;
int buttonLastReading = HIGH;
unsigned long buttonLastChangeTime = 0;
static const unsigned long BUTTON_DEBOUNCE_MS = 40;

uint32_t pulseWidthToDuty(uint32_t pulseWidthUs) {
  const uint32_t maxDuty = (1UL << SERVO_PWM_RESOLUTION) - 1;
  return (pulseWidthUs * maxDuty) / 20000UL;
}

void writeServoAngle(uint8_t angleDegrees) {
  const uint32_t pulseWidthUs = map(angleDegrees, 0, 180, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);
  ledcWrite(SERVO_PWM_CHANNEL, pulseWidthToDuty(pulseWidthUs));
}

void showHelmetState(bool open) {
  const char *statusText = open ? "Helmet open 90" : "Helmet closed 0";
  char lineBuffer[17];

  snprintf(lineBuffer, sizeof(lineBuffer), "%-15s", statusText);

  lcd.setCursor(0, 1);
  lcd.print(lineBuffer);
  lcd.setCursor(15, 1);
  lcd.print(spinnerChars[spinnerIndex]);
}

void setHelmetOpen(bool open) {
  helmetOpen = open;
  writeServoAngle(open ? 90 : 0);
  showHelmetState(open);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  ledcSetup(SERVO_PWM_CHANNEL, SERVO_PWM_FREQUENCY, SERVO_PWM_RESOLUTION);
  ledcAttachPin(SERVO_PIN, SERVO_PWM_CHANNEL);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Hello, World!");

  setHelmetOpen(false);

  Serial.println("LCD ready on SDA 21 / SCL 22");
  Serial.println("Button on GPIO 19, servo on GPIO 18");
}

void loop() {
  int buttonReading = digitalRead(BUTTON_PIN);

  if (buttonReading != buttonLastReading) {
    buttonLastChangeTime = millis();
    buttonLastReading = buttonReading;
  }

  if ((millis() - buttonLastChangeTime) > BUTTON_DEBOUNCE_MS && buttonReading != buttonStableState) {
    buttonStableState = buttonReading;

    if (buttonStableState == LOW) {
      setHelmetOpen(!helmetOpen);
    }
  }

  unsigned long now = millis();

  if (now - lastSpinnerUpdate >= 200) {
    lastSpinnerUpdate = now;
    spinnerIndex = (spinnerIndex + 1) % 4;

    lcd.setCursor(15, 1);
    lcd.print(spinnerChars[spinnerIndex]);
  }
}
