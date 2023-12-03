#include "Enes100.h"
#include <math.h>

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
const int AechoPin = 13;  // Echo pin

float x, y, t, distance;

// Broad-Scope Function Prototypes
void forward();
void backward();
void left_turn();
void right_turn();
void stop();
float distance1(int trigPin, int echoPin);
void turn_to(double target_angle);
void get_to_point(double target_x, double target_y);

void setup() {
  // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
  delay(2000);
  Enes100.begin("Ban-anna Mike-anics", WATER, 10, 11, 10);
  Enes100.println("Connected...");
  delay(2000);

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

  Serial.begin(9600);
}

void loop() {
  get_to_point(0.53, 0.53);
}

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
  digitalWrite(leftRearPin1, HIGH);
  ;
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
  digitalWrite(trigPin, LOW);  // Pulsing trigger pin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);  // Measure time taken for echo pin to go HIGH
  int distance = (duration / 29) / 2;      // Speed of sound = 29 microsec/cm. Divided by 2 b/c back and forth.

  return distance;
}

void turn_to(double target_angle) {
  int turnDelay = 5;
  bool keep_turning = true;
  double tolerance = 0.4;

  t = Enes100.getTheta();
  while (keep_turning == true) {
    t = Enes100.getTheta();
    if (((t < target_angle + tolerance) && (t > target_angle - tolerance))) {
      stop();
      keep_turning = false;
    } else {
      right_turn();
      delay(turnDelay);
      stop();
      delay(turnDelay);
    }
  }
}

void get_to_point(double target_x, double target_y) {
  float cur_x, cur_y;
  double heading, tolerance;
  tolerance = 0.4;

  Enes100.updateLocation();
  cur_x = Enes100.location.x;
  cur_y = Enes100.location.y;

  heading = atan2(target_y - cur_y, target_x - cur_x);

  turn_to(heading);

  bool keep_driving = true;
  unsigned long current_time;
  unsigned long last_recalculation = 0;
  unsigned long recalculation_interval = 2000;

  while (keep_driving = true) {
    forward();
    if (cur_x > (cur_x - tolerance) && cur_x < (cur_x + tolerance) && cur_y > (cur_y - tolerance) && cur_y < (cur_y + tolerance)) {
      stop();
      keep_driving = false;
    } else {
      forward();
      current_time = millis();
      if (current_time - last_recalculation >= recalculation_interval) {
        stop();
        heading = atan2(target_y - cur_y, target_x - cur_x);
        turn_to(heading);
        last_recalculation = current_time;
      }
    }
  }

  stop();
}