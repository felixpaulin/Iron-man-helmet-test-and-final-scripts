#include <ESP32Servo.h>
#include <Arduino.h>

const int BUTTON_PIN = 12;
const int RIGHT_CHEEK_PIN = 18;
const int LEFT_CHEEK_PIN = 19;
const int MIDDLE_TOP_PIN = 23;
const int BOTTOM_SERVO_PIN = 26;

Servo right_cheek;
Servo left_cheek;
Servo middle_top;
Servo bottom_servo;

bool servoAt50 = false;
bool lastButtonState = HIGH;

const int STEP_DELAY_MS = 10;
const int MIDDLE_TOP_DELAY_MS = 300;
const int BOTTOM_DELAY_MS = 300;

// Raw per-servo values so you can manually control each side.
const int RIGHT_CHEEK_CLOSED = 117;
const int RIGHT_CHEEK_OPEN = 50;
const int LEFT_CHEEK_CLOSED = 50;
const int LEFT_CHEEK_OPEN = 117;
const int MIDDLE_TOP_CLOSED = 150;
const int MIDDLE_TOP_OPEN = 30;
const int BOTTOM_SERVO_CLOSED = 40;
const int BOTTOM_SERVO_OPEN = 130;

void moveBothServosTo(int rightAngle, int leftAngle) {
  right_cheek.attach(RIGHT_CHEEK_PIN);
  left_cheek.attach(LEFT_CHEEK_PIN);

  for (int i = 0; i <= 10; i++) {
    right_cheek.write(rightAngle);
    left_cheek.write(leftAngle);
    delay(STEP_DELAY_MS);
  }

  right_cheek.detach();
  left_cheek.detach();
}

void moveMiddleTopTo(int angle) {
  middle_top.attach(MIDDLE_TOP_PIN);
  middle_top.write(angle);
  delay(250);
  middle_top.detach();
}

void moveBottomServoTo(int angle) {
  bottom_servo.attach(BOTTOM_SERVO_PIN);
  bottom_servo.write(angle);
  delay(250);
  bottom_servo.detach();
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Start by moving once to the closed positions, then detach
  moveBothServosTo(RIGHT_CHEEK_CLOSED, LEFT_CHEEK_CLOSED);
  moveMiddleTopTo(MIDDLE_TOP_CLOSED);
  moveBottomServoTo(BOTTOM_SERVO_CLOSED);
  delay(500);
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(20);
    if (digitalRead(BUTTON_PIN) == LOW) {
      servoAt50 = !servoAt50;

      if (servoAt50) {
        moveBothServosTo(RIGHT_CHEEK_OPEN, LEFT_CHEEK_OPEN);
        delay(MIDDLE_TOP_DELAY_MS);
        moveMiddleTopTo(MIDDLE_TOP_OPEN);
        delay(BOTTOM_DELAY_MS);
        moveBottomServoTo(BOTTOM_SERVO_OPEN);
      } else {
        moveBottomServoTo(BOTTOM_SERVO_CLOSED);
        delay(BOTTOM_DELAY_MS);
        moveMiddleTopTo(MIDDLE_TOP_CLOSED);
        delay(MIDDLE_TOP_DELAY_MS);
        moveBothServosTo(RIGHT_CHEEK_CLOSED, LEFT_CHEEK_CLOSED);
      }
    }
  }

  lastButtonState = buttonState;
}
