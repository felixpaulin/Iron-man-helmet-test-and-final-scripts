#include <ESP32Servo.h> // ESP32-compatible servo control library
#include <Arduino.h>     // Arduino core functions and constants

const int buttonPin = 12;            // pin for the button input
const int rightCheekPin = 18;        // pin for the right cheek servo
const int leftCheekPin = 19;         // pin for the left cheek servo
const int middleTopPin = 23;         // pin for the middle top servo
const int bottomServoPin = 26;       // pin for the bottom servo

Servo rightCheek;                    // servo object for right cheek
Servo leftCheek;                     // servo object for left cheek
Servo middleTop;                     // servo object for middle top
Servo bottomServo;                   // servo object for the bottom servo

bool isOpenPosition = false;         // tracks whether the helmet is currently open
bool lastButtonState = HIGH;         // remembers the last button state for debounce

const int stepDelayMs = 1;          // delay between each incremental servo step
const int middleTopDelayMs = 300;    // delay after cheeks before moving middle top
const int bottomDelayMs = 300;       // delay after middle top before moving bottom servo

const int rightCheekClosed = 117;    // closed angle for right cheek servo
const int rightCheekOpen = 50;       // open angle for right cheek servo
const int leftCheekClosed = 50;      // closed angle for left cheek servo
const int leftCheekOpen = 117;       // open angle for left cheek servo
const int middleTopClosed = 150;     // closed angle for middle top servo
const int middleTopOpen = 30;        // open angle for middle top servo
const int bottomServoClosed = 40;   // closed angle for bottom servo
const int bottomServoOpen = 135;      // open angle for bottom servo

int rightCheekAngle = rightCheekClosed;  // current right cheek angle state
int leftCheekAngle = leftCheekClosed;    // current left cheek angle state
int middleTopAngle = middleTopClosed;    // current middle top angle state
int bottomServoAngle = bottomServoClosed; // current bottom servo angle state

void moveServoTo(Servo &servo, int &currentAngle, int targetAngle, int pin) {
  servo.attach(pin);                    // attach the servo to its pin so it can receive PWM

  while (currentAngle != targetAngle) { // move one degree at a time until we reach the target
    if (currentAngle < targetAngle) {   // if we need to increase the angle
      currentAngle++;
    } else {                            // if we need to decrease the angle
      currentAngle--;
    }

    servo.write(currentAngle);         // command the servo to the next step angle
    delay(stepDelayMs);                // wait so the servo moves more slowly and smoothly
  }

  servo.detach();                      // detach the servo to reduce buzzing once movement is complete
}

void moveBothServosTo(int rightTarget, int leftTarget) {
  rightCheek.attach(rightCheekPin);    // attach both cheek servos before moving them together
  leftCheek.attach(leftCheekPin);

  while (rightCheekAngle != rightTarget || leftCheekAngle != leftTarget) {
    if (rightCheekAngle < rightTarget) { // move right cheek toward its target
      rightCheekAngle++;
    } else if (rightCheekAngle > rightTarget) {
      rightCheekAngle--;
    }

    if (leftCheekAngle < leftTarget) {   // move left cheek toward its target
      leftCheekAngle++;
    } else if (leftCheekAngle > leftTarget) {
      leftCheekAngle--;
    }

    rightCheek.write(rightCheekAngle);  // write the current right cheek angle
    leftCheek.write(leftCheekAngle);    // write the current left cheek angle
    delay(stepDelayMs);                 // delay between each step for smooth motion
  }

  rightCheek.detach();                 // detach the right cheek servo when done
  leftCheek.detach();                  // detach the left cheek servo when done
}

void moveMiddleTopTo(int targetAngle) {
  moveServoTo(middleTop, middleTopAngle, targetAngle, middleTopPin); // move middle top servo with smooth steps
}

void moveBottomServoTo(int targetAngle) {
  moveServoTo(bottomServo, bottomServoAngle, targetAngle, bottomServoPin); // move bottom servo with smooth steps
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);     // set button pin as input with internal pull-up resistor

  rightCheek.attach(rightCheekPin);    // initialize right cheek in closed position
  leftCheek.attach(leftCheekPin);
  rightCheek.write(rightCheekAngle);
  leftCheek.write(leftCheekAngle);
  delay(250);                          // allow initial position to settle
  rightCheek.detach();
  leftCheek.detach();

  moveMiddleTopTo(middleTopClosed);    // initialize middle top in closed position smoothly
  moveBottomServoTo(bottomServoClosed); // initialize bottom servo in closed position smoothly
  delay(500);                          // wait after the initial setup is complete
}

void loop() {
  bool buttonState = digitalRead(buttonPin); // read the current button state

  if (buttonState == LOW && lastButtonState == HIGH) { // detect button press edge
    delay(20);                         // debounce delay to avoid false triggers
    if (digitalRead(buttonPin) == LOW) { // confirm button is still pressed
      isOpenPosition = !isOpenPosition; // toggle the open/closed state

      if (isOpenPosition) {
        moveBothServosTo(rightCheekOpen, leftCheekOpen); // move both cheek servos together first
        delay(middleTopDelayMs);                         // wait before moving the middle top servo
        moveMiddleTopTo(middleTopOpen);                  // move middle top to open angle
        delay(bottomDelayMs);                            // wait before moving the bottom servo
        moveBottomServoTo(bottomServoOpen);              // move the bottom servo last
      } else {
        moveBottomServoTo(bottomServoClosed);            // move the bottom servo first when closing
        delay(bottomDelayMs);                            // wait before moving the middle top servo
        moveMiddleTopTo(middleTopClosed);                // move the middle top to closed angle
        delay(middleTopDelayMs);                         // wait before moving the cheeks
        moveBothServosTo(rightCheekClosed, leftCheekClosed); // close both cheek servos together
      }
    }
  }

  lastButtonState = buttonState;      // save the current button state for the next loop iteration
}
