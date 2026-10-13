// ======================================================
//              2-SENSOR LINE FOLLOWER
//          Arduino UNO / Arduino NANO
//                 L298N DRIVER
// ======================================================
//
// IR SENSOR:
// Left  IR OUT  -> D2
// Right IR OUT  -> D3
//
// L298N:
// ENA -> D5
// IN1 -> D7
// IN2 -> D8
// IN3 -> D9
// IN4 -> D10
// ENB -> D6
//
// IR VCC -> Arduino 5V
// IR GND -> Arduino GND
// L298N GND -> Arduino GND
//
// ======================================================


// ---------------- IR SENSORS ----------------

#define LEFT_SENSOR   2
#define RIGHT_SENSOR  3


// ---------------- L298N ----------------

#define ENA 5
#define IN1 7
#define IN2 8

#define IN3 9
#define IN4 10
#define ENB 6


// ======================================================
//                 SENSOR CONFIGURATION
// ======================================================

// Most IR line sensors:
// LOW  = BLACK line
// HIGH = WHITE surface

#define LINE_DETECTED LOW


// ======================================================
//                  SPEED SETTINGS
// ======================================================

// Start with these values.
// Tune them according to your motors and arena.

int baseSpeed = 60;

int correction = 45;

int turnSpeed = 60;


// ======================================================
//             LAST DIRECTION MEMORY
// ======================================================

// -1 = last saw line on LEFT
//  1 = last saw line on RIGHT
//  0 = straight

int lastDirection = 0;


// ======================================================
//                       SETUP
// ======================================================

void setup() {

  // IR sensors
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);


  // L298N
  pinMode(ENA, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENB, OUTPUT);


  // Initially stop
  stopMotors();

  delay(800);
}


// ======================================================
//                  LEFT MOTOR FORWARD
// ======================================================

void leftForward(int speed) {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  analogWrite(ENA, constrain(speed, 0, 255));
}


// ======================================================
//                 RIGHT MOTOR FORWARD
// ======================================================

void rightForward(int speed) {

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENB, constrain(speed, 0, 255));
}


// ======================================================
//                      STOP
// ======================================================

void stopMotors() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


// ======================================================
//                  HARD LEFT
// ======================================================

void hardLeft() {

  // Left motor backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Right motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, turnSpeed);
  analogWrite(ENB, turnSpeed);
}


// ======================================================
//                  HARD RIGHT
// ======================================================

void hardRight() {

  // Left motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Right motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, turnSpeed);
  analogWrite(ENB, turnSpeed);
}


// ======================================================
//                 LINE FOLLOWING
// ======================================================

void loop() {

  int leftState  = digitalRead(LEFT_SENSOR);
  int rightState = digitalRead(RIGHT_SENSOR);


  // ==================================================
  // CASE 1
  // BOTH ON WHITE
  //
  // Robot has probably LOST the line
  // ==================================================

  if (leftState != LINE_DETECTED &&
      rightState != LINE_DETECTED) {


    // Last line was towards LEFT
    if (lastDirection == -1) {

      hardLeft();
    }


    // Last line was towards RIGHT
    else if (lastDirection == 1) {

      hardRight();
    }


    // No previous direction
    else {

      leftForward(baseSpeed);
      rightForward(baseSpeed);
    }
  }


  // ==================================================
  // CASE 2
  // LEFT SENSOR ON BLACK
  // RIGHT SENSOR ON WHITE
  //
  // Move LEFT
  // ==================================================

  else if (leftState == LINE_DETECTED &&
           rightState != LINE_DETECTED) {


    // Slow left motor
    // Speed up right motor

    leftForward(baseSpeed - correction);
    rightForward(baseSpeed + correction);

    lastDirection = -1;
  }


  // ==================================================
  // CASE 3
  // RIGHT SENSOR ON BLACK
  // LEFT SENSOR ON WHITE
  //
  // Move RIGHT
  // ==================================================

  else if (leftState != LINE_DETECTED &&
           rightState == LINE_DETECTED) {


    // Speed up left motor
    // Slow right motor

    leftForward(baseSpeed + correction);
    rightForward(baseSpeed - correction);

    lastDirection = 1;
  }


  // ==================================================
  // CASE 4
  // BOTH ON BLACK
  //
  // Usually:
  // - straight line
  // - intersection
  // - thick line
  // ==================================================

  else {

    leftForward(baseSpeed);
    rightForward(baseSpeed);

    lastDirection = 0;
  }


  // Very small delay for stable response
  delay(2);
}