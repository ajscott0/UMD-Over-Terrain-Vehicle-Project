#include "water_code.h"
#include "Enes100.h"

// Broad-Scope Function Implementations

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
float distance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); // Pulsing trigger pin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH); // Measure time taken for echo pin to go HIGH
  int distance = (duration / 29) / 2;  // Speed of sound = 29 microsec/cm. Divided by 2 b/c back and forth.

  return distance;
}

int tds_go() {
  const float referenceVoltage = 5.0;
  float tdsAnalogInput = analogRead(tdsPin);
  // Normalize reading in Arduino analog value range: (0, 1023) then convert into ppm.
  float tdsValue = (tdsAnalogInput / 1024.0) * referenceVoltage * 500.0;

  if (tdsValue <= 450) {  // Fresh
    return 0;
  } else {  // Salty
    return 1;
  }
}

int turbidity_go() {
  float turbidityValue = analogRead(turbidityPin);

  if (turbidityValue >= 875) {  // Not polluted
    return 0;
  } else {  // Polluted
    return 1;
  }
}

float depth() {
  float dist_to_water = distance(BtrigPin, BechoPin);
  float depth = ARM_HEIGHT - POOL_THICK - (dist_to_water * 10);

  return depth;
}

// Turn pump on and run for 10 seconds before stopping
void pump_go() {
  analogWrite(pumpPin1, 255);
  analogWrite(pumpPin2, 0);
  delay(10000);
  analogWrite(pumpPin1, 0);
  analogWrite(pumpPin2, 0);
}

// Specific Mission Phase Functions

int get_to_site() {
  float x, y, t, distance; 

  x = Enes100.getX();  // 0-4 meters
  y = Enes100.getY();  // 0-2 meters
  t = Enes100.getTheta();  // -pi to +pi in radians
  distance = distance(AtrigPin, AechoPin);

  if (y >= 1) {
    while (t != -PI/2) {  // How to update this constanty??
      right_turn();
      t = Enes100.getTheta();
    }
  } else if (y < 1) {
    while (t != PI/2) {
      right_turn();
      t = Enes100.getTheta();
    }
  }

  while (distance > 3) {  // How to update this constanty??
    forward();
    distance = distance(AtrigPin, AechoPin);
  }
  stop();
}

int collect_water() {

}