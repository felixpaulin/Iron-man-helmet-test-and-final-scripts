#include <ESP32Servo.h>

const int BUTTON_PIN = 12;
const int SERVO_PIN = 18;

Servo myServo;

bool servoAt40 = false;
bool lastButtonState = HIGH;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  myServo.attach(SERVO_PIN);

  // Start at 0 degrees
  myServo.write(0);
  delay(500);
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  // Detect a press (LOW because INPUT_PULLUP)
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(20); // simple debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      servoAt40 = !servoAt40;
      myServo.write(servoAt40 ? 40 : 0);
    }
  }

  lastButtonState = buttonState;
}
