#include "Enes100.h"

// Motor driver pins
const int rightFrontPin1 = 2;
const int rightFrontPin2 = 3;
const int rightRearPin1 = 4;
const int rightRearPin2 = 5;

const int leftFrontPin1 = 6;
const int leftFrontPin2 = 7;
const int leftRearPin1 = 8;
const int leftRearPin2 = 9;

// Distance sensor pins
const int AtrigPin = 12;  // Trigger pin
const int AechoPin = 13; // Echo pin

float x, y, t, distance;

// Broad-Scope Function Prototypes
void forward();
void backward();
void left_turn();
void right_turn();
void stop();
float distance1(int trigPin, int echoPin);

void setup() {
  // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
  Enes100.begin("Ban-anna Mike-anics", WATER, 10, 11, 10);
  Enes100.println("Connected...");

  pinMode(AtrigPin, OUTPUT);
  pinMode(AechoPin, INPUT);

  pinMode(rightFrontPin1, OUTPUT);
  pinMode(rightFrontPin2, OUTPUT);
  pinMode(rightRearPin1, OUTPUT);
  pinMode(rightRearPin2, OUTPUT);

  pinMode(leftFrontPin1, OUTPUT);
  pinMode(leftFrontPin2, OUTPUT);
  pinMode(leftRearPin1, OUTPUT);
  pinMode(leftRearPin2, OUTPUT);

  Serial.begin(9600); // Initializing baud rate at 9600 bits/sec
}

void loop() {
  // FIRST test each movement function! (one at a time)
  forward();
  backward();
  left_turn();
  right_turn();
  stop();
  // const float tolerance = 20;
  // x = Enes100.getX();  // 0-4 meters
  // y = Enes100.getY();  // 0-2 meters
  // t = Enes100.getTheta();  // -pi to +pi in radians
  // distance = distance1(AtrigPin, AechoPin);

  // if (y >= 1) {
  //   while (t > -1.50 && t < -1.64) {
  //     right_turn();
  //     t = Enes100.getTheta();
  //   }
  //   stop();
  // } else if (y < 1) {
  //   while (t < 1.50 && t > 1.64) {
  //     right_turn();
  //     t = Enes100.getTheta();
  //   }
  //   stop();
  // }
  // Rest of get_to-site() code
}

// Motor driver function implementations
/* samePin1&2 should NEVER be same at same time */
void forward() {
  digitalWrite(rightFrontPin1, HIGH);
  digitalWrite(rightFrontPin2, LOW);
  digitalWrite(rightRearPin1, HIGH);
  digitalWrite(rightRearPin2, LOW);

  digitalWrite(leftFrontPin1, LOW);
  digitalWrite(leftFrontPin2, HIGH);
  digitalWrite(leftRearPin1, LOW);
  digitalWrite(leftRearPin2, HIGH);
}

void backward() {
  digitalWrite(rightFrontPin1, LOW);
  digitalWrite(rightFrontPin2, HIGH);
  digitalWrite(rightRearPin1, LOW);
  digitalWrite(rightRearPin2, HIGH);

  digitalWrite(leftFrontPin1, HIGH);
  digitalWrite(leftFrontPin2, LOW);
  digitalWrite(leftRearPin1, HIGH);
  digitalWrite(leftRearPin2, LOW);
}

void left_turn() {
  digitalWrite(rightFrontPin1, HIGH);
  digitalWrite(rightFrontPin2, LOW);
  digitalWrite(rightRearPin1, HIGH);
  digitalWrite(rightRearPin2, LOW);

  digitalWrite(leftFrontPin1, HIGH);
  digitalWrite(leftFrontPin2, LOW);
  digitalWrite(leftRearPin1, HIGH);;
  digitalWrite(leftRearPin2, LOW);
}

void right_turn() {
  digitalWrite(rightFrontPin1, LOW);
  digitalWrite(rightFrontPin2, HIGH);
  digitalWrite(rightRearPin1, LOW);
  digitalWrite(rightRearPin2, HIGH);

  digitalWrite(leftFrontPin1, LOW);
  digitalWrite(leftFrontPin2, HIGH);
  digitalWrite(leftRearPin1, LOW);
  digitalWrite(leftRearPin2, HIGH);
}

void stop() {
  digitalWrite(rightFrontPin1, LOW);
  digitalWrite(rightFrontPin2, LOW);
  digitalWrite(rightRearPin1, LOW);
  digitalWrite(rightRearPin2, LOW);

  digitalWrite(leftFrontPin1, LOW);
  digitalWrite(leftFrontPin2, LOW);
  digitalWrite(leftRearPin1, LOW);
  digitalWrite(leftRearPin2, LOW);
}

// Sensor function implementations
float distance1(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); // Pulsing trigger pin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH); // Measure time taken for echo pin to go HIGH
  int distance = (duration / 29) / 2;  // Speed of sound = 29 microsec/cm. Divided by 2 b/c back and forth.

  return distance;
}