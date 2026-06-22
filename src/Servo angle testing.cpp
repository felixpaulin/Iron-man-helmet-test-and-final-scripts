#include <ESP32Servo.h>

// ----------------------
// CHANGE THESE PINS FIRST
// These are the only pins you should edit for your wiring.
// Button pins must be connected to INPUT_PULLUP buttons.
// Servo pins must be connected to the servo signal wires.
// ----------------------
const int Integral_open_button = 12;   // CHANGE: button pin for the 2-servo sequence
const int Section_open_button = 13;   // CHANGE: button pin for the 6-servo sequence

const int SERVO_1_PIN = 18;          // CHANGE: signal pin for servo 1
const int SERVO_2_PIN = 19;          // CHANGE: signal pin for servo 2
const int SERVO_3_PIN = 21;          // CHANGE: signal pin for servo 3
const int SERVO_4_PIN = 22;          // CHANGE: signal pin for servo 4
const int SERVO_5_PIN = 23;          // CHANGE: signal pin for servo 5
const int SERVO_6_PIN = 25;          // CHANGE: signal pin for servo 6

// ----------------------
// Servo objects
// ----------------------
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

// ----------------------
// CHANGE THESE ANGLES AND TIMING
// OPEN_ANGLE = how far the servos move when opening
// CLOSED_ANGLE = resting position (usually 0)
// STEP_DELAY_MS = time between each tiny movement step
// ----------------------
const int STEP_DELAY_MS = 15;        // CHANGE: slower = smoother, faster = quicker
const int OPEN_ANGLE = 40;           // CHANGE: opening angle for the first pair / general open position
const int SECOND_OPEN_ANGLE = 80;    // CHANGE: optional extra open position if you want a bigger move
const int CLOSED_ANGLE = 0;          // CHANGE: resting angle (usually leave at 0)

// These names are just labels for the sequence order.
// You can ignore them unless you want clearer code later.
const int SERVO_PAIR_A_FIRST = 1;
const int SERVO_PAIR_A_SECOND = 2;
const int SERVO_PAIR_B_FIRST = 3;
const int SERVO_PAIR_B_SECOND = 4;

bool twoServoOpen = false;
bool sixServoOpen = false;

bool twoServoButtonLast = HIGH;
bool sixServoButtonLast = HIGH;

bool twoServoRunning = false;
bool sixServoRunning = false;

int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;
int pos6 = 0;

// ----------------------
// CHANGE THESE MOVEMENT RULES IF NEEDED
// These control how each servo moves step by step.
// ----------------------
void moveSingleServo(Servo &servo, int &currentPos, int targetPos) {
  if (currentPos < targetPos) {
    for (int i = currentPos; i <= targetPos; i++) {
      servo.write(i);
      currentPos = i;
      delay(STEP_DELAY_MS);
    }
  } else if (currentPos > targetPos) {
    for (int i = currentPos; i >= targetPos; i--) {
      servo.write(i);
      currentPos = i;
      delay(STEP_DELAY_MS);
    }
  }
}

void movePairSync(Servo &servoA, Servo &servoB, int &posA, int &posB, int targetPos) {
  while (posA != targetPos || posB != targetPos) {
    if (posA != targetPos) {
      posA += (posA < targetPos) ? 1 : -1;
      servoA.write(posA);
    }

    if (posB != targetPos) {
      posB += (posB < targetPos) ? 1 : -1;
      servoB.write(posB);
    }

    delay(STEP_DELAY_MS);
  }
}

// ----------------------
// CHANGE THIS FUNCTION IF YOU WANT A DIFFERENT 2-SERVO ORDER
// This runs when the 2-servo button is pressed.
// ----------------------
void runTwoServoSequence() {
  if (twoServoRunning) return;
  twoServoRunning = true;

  if (!twoServoOpen) {
    // Open sequence
    movePairSync(servo1, servo2, pos1, pos2, OPEN_ANGLE);
  } else {
    // Close sequence in reverse
    movePairSync(servo1, servo2, pos1, pos2, CLOSED_ANGLE);
  }

  twoServoOpen = !twoServoOpen;
  twoServoRunning = false;
}

// ----------------------
// CHANGE THIS FUNCTION IF YOU WANT A DIFFERENT 6-SERVO ORDER
// Current order:
// 1) servo1 + servo2   (same pair used in the 2-servo sequence)
// 2) servo3
// 3) servo4
// 4) servo5 + servo6   (same pair used in the 2-servo sequence)
// ----------------------
void runSixServoSequence() {
  if (sixServoRunning) return;
  sixServoRunning = true;

  if (!sixServoOpen) {
    // New pattern:
    // 1) servo1 + servo2 move together
    // 2) servo3 moves
    // 3) servo4 moves
    // 4) servo5 + servo6 move together
    // NOTE: the last two servos here are the same pair as the 2-servo sequence
    movePairSync(servo1, servo2, pos1, pos2, OPEN_ANGLE);
    moveSingleServo(servo3, pos3, OPEN_ANGLE);
    moveSingleServo(servo4, pos4, OPEN_ANGLE);
    movePairSync(servo5, servo6, pos5, pos6, OPEN_ANGLE);
  } else {
    // Close sequence in reverse
    movePairSync(servo5, servo6, pos5, pos6, CLOSED_ANGLE);
    moveSingleServo(servo4, pos4, CLOSED_ANGLE);
    moveSingleServo(servo3, pos3, CLOSED_ANGLE);
    movePairSync(servo1, servo2, pos1, pos2, CLOSED_ANGLE);
  }

  sixServoOpen = !sixServoOpen;
  sixServoRunning = false;
}

void setup() {
  pinMode(Integral_open_button, INPUT_PULLUP);
  pinMode(Section_open_button, INPUT_PULLUP);

  servo1.attach(SERVO_1_PIN);
  servo2.attach(SERVO_2_PIN);
  servo3.attach(SERVO_3_PIN);
  servo4.attach(SERVO_4_PIN);
  servo5.attach(SERVO_5_PIN);
  servo6.attach(SERVO_6_PIN);

  // Start all servos at 0
  servo1.write(CLOSED_ANGLE);
  servo2.write(CLOSED_ANGLE);
  servo3.write(CLOSED_ANGLE);
  servo4.write(CLOSED_ANGLE);
  servo5.write(CLOSED_ANGLE);
  servo6.write(CLOSED_ANGLE);
  delay(500);
}

void loop() {
  bool twoButtonState = digitalRead(Integral_open_button);
  bool sixButtonState = digitalRead(Section_open_button);

  // Button 1 press handling
  if (twoButtonState == LOW && twoServoButtonLast == HIGH) {
    delay(20);
    if (digitalRead(Integral_open_button) == LOW) {
      runTwoServoSequence();
    }
  }
  twoServoButtonLast = twoButtonState;

  // Button 2 press handling
  if (sixButtonState == LOW && sixServoButtonLast == HIGH) {
    delay(20);
    if (digitalRead(Section_open_button) == LOW) {
      runSixServoSequence();
    }
  }
  sixServoButtonLast = sixButtonState;
}
