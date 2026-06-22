#include <ESP32Servo.h>

// ----------------------
// CHANGE THESE PINS FIRST
// ----------------------
const int INTEGRAL_BUTTON_PIN = 12;    // CHANGE: button for the top pair sequence
const int SECTIONAL_BUTTON_PIN = 13;   // CHANGE: button for the full sectional sequence

const int TOP_LEFT_PIN = 18;           // CHANGE: pin for top left servo
const int TOP_RIGHT_PIN = 19;          // CHANGE: pin for top right servo
const int CHEEK_LEFT_PIN = 21;         // CHANGE: pin for left cheek servo
const int CHEEK_RIGHT_PIN = 22;        // CHANGE: pin for right cheek servo
const int MIDDLE_TOP_PIN = 23;         // CHANGE: pin for middle top servo
const int BOTTOM_PIN = 25;             // CHANGE: pin for bottom servo

// ----------------------
// SERVO OBJECTS (using your helmet names)
// ----------------------
Servo topLeft;
Servo topRight;
Servo cheekLeft;
Servo cheekRight;
Servo middleTop;
Servo bottomServo;

// ----------------------
// CHANGE THESE ANGLES / TIMING
// ----------------------
const int STEP_DELAY_MS = 15;          // CHANGE: higher = slower motion
const int CLOSED_ANGLE = 0;            // CHANGE: closed/rest angle

const int TOP_LEFT_ANGLE = 40;         // CHANGE: angle for top left servo
const int TOP_RIGHT_ANGLE = 40;        // CHANGE: angle for top right servo
const int CHEEK_LEFT_ANGLE = 40;       // CHANGE: angle for left cheek servo
const int CHEEK_RIGHT_ANGLE = 40;      // CHANGE: angle for right cheek servo
const int MIDDLE_TOP_ANGLE = 40;       // CHANGE: angle for middle top servo
const int BOTTOM_ANGLE = 40;           // CHANGE: angle for bottom servo

// ----------------------
// STATE TRACKING
// ----------------------
bool integralIsOpen = false;
bool sectionalIsOpen = false;

bool integralButtonLast = HIGH;
bool sectionalButtonLast = HIGH;

// ----------------------
// HELPER: move one servo slowly
// ----------------------
void moveServoTo(Servo &servo, int currentAngle, int targetAngle) {
  if (currentAngle < targetAngle) {
    for (int i = currentAngle; i <= targetAngle; i++) {
      servo.write(i);
      delay(STEP_DELAY_MS);
    }
  } else if (currentAngle > targetAngle) {
    for (int i = currentAngle; i >= targetAngle; i--) {
      servo.write(i);
      delay(STEP_DELAY_MS);
    }
  }
}

// ----------------------
// INTEGRAL OPEN / CLOSE
// Two top servos move together
// ----------------------
void integralOpen() {
  moveServoTo(topLeft, 0, TOP_LEFT_ANGLE);
  moveServoTo(topRight, 0, TOP_RIGHT_ANGLE);
  integralIsOpen = true;
}

void integralClose() {
  moveServoTo(topLeft, TOP_LEFT_ANGLE, CLOSED_ANGLE);
  moveServoTo(topRight, TOP_RIGHT_ANGLE, CLOSED_ANGLE);
  integralIsOpen = false;
}

// ----------------------
// SECTIONAL OPEN / CLOSE
// Sequence you described:
// 1) cheek pair move together
// 2) middle top moves
// 3) bottom moves
// 4) top pair move together
// ----------------------
void sectionalOpen() {
  moveServoTo(cheekLeft, 0, CHEEK_LEFT_ANGLE);
  moveServoTo(cheekRight, 0, CHEEK_RIGHT_ANGLE);
  moveServoTo(middleTop, 0, MIDDLE_TOP_ANGLE);
  moveServoTo(bottomServo, 0, BOTTOM_ANGLE);
  moveServoTo(topLeft, 0, TOP_LEFT_ANGLE);
  moveServoTo(topRight, 0, TOP_RIGHT_ANGLE);
  sectionalIsOpen = true;
}

void sectionalClose() {
  moveServoTo(topRight, TOP_RIGHT_ANGLE, CLOSED_ANGLE);
  moveServoTo(topLeft, TOP_LEFT_ANGLE, CLOSED_ANGLE);
  moveServoTo(bottomServo, BOTTOM_ANGLE, CLOSED_ANGLE);
  moveServoTo(middleTop, MIDDLE_TOP_ANGLE, CLOSED_ANGLE);
  moveServoTo(cheekRight, CHEEK_RIGHT_ANGLE, CLOSED_ANGLE);
  moveServoTo(cheekLeft, CHEEK_LEFT_ANGLE, CLOSED_ANGLE);
  sectionalIsOpen = false;
}

void setup() {
  pinMode(INTEGRAL_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SECTIONAL_BUTTON_PIN, INPUT_PULLUP);

  topLeft.attach(TOP_LEFT_PIN);
  topRight.attach(TOP_RIGHT_PIN);
  cheekLeft.attach(CHEEK_LEFT_PIN);
  cheekRight.attach(CHEEK_RIGHT_PIN);
  middleTop.attach(MIDDLE_TOP_PIN);
  bottomServo.attach(BOTTOM_PIN);

  // Start all servos at closed position
  topLeft.write(CLOSED_ANGLE);
  topRight.write(CLOSED_ANGLE);
  cheekLeft.write(CLOSED_ANGLE);
  cheekRight.write(CLOSED_ANGLE);
  middleTop.write(CLOSED_ANGLE);
  bottomServo.write(CLOSED_ANGLE);
  delay(500);
}

void loop() {
  bool integralButtonState = digitalRead(INTEGRAL_BUTTON_PIN);
  bool sectionalButtonState = digitalRead(SECTIONAL_BUTTON_PIN);

  // Integral button handling
  if (integralButtonState == LOW && integralButtonLast == HIGH) {
    delay(20);
    if (digitalRead(INTEGRAL_BUTTON_PIN) == LOW) {
      if (integralIsOpen) {
        integralClose();
      } else {
        integralOpen();
      }
    }
  }
  integralButtonLast = integralButtonState;

  // Sectional button handling
  if (sectionalButtonState == LOW && sectionalButtonLast == HIGH) {
    delay(20);
    if (digitalRead(SECTIONAL_BUTTON_PIN) == LOW) {
      if (sectionalIsOpen) {
        sectionalClose();
      } else {
        sectionalOpen();
      }
    }
  }
  sectionalButtonLast = sectionalButtonState;
}
