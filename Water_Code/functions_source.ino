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
  float dist_to_water = distance1(BtrigPin, BechoPin);
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
  distance = distance1(AtrigPin, AechoPin);

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
    distance = distance1(AtrigPin, AechoPin);
  }
  stop();
  return 1;
}

int collect_water() {
  pump_go();
  return 1;
}

int obtain_data() {
  // Transmit the state of the pool
  if (tds_go() && turbidity_go()) {
    Enes100.mission(WATER_TYPE, SALT_POLLUTED);
  } else if (tds_go() && !turbidity_go()) {
    Enes100.mission(WATER_TYPE, SALT_UNPOLLUTED);
  } else if (!tds_go() && turbidity_go()) {
    Enes100.mission(WATER_TYPE, FRESH_POLLUTED);
  } else if (!tds_go() && !turbidity_go()) {
    Enes100.mission(WATER_TYPE, FRESH_UNPOLLUTED);
  }

  // Transmit the depth of the pool in mm (20, 30, or 40)
  Enes100.mission(DEPTH, depth());
  
  return 1;
}

int get_to_destination() {
  float x, y, t distance;
  x = Enes100.getX(); // All my initial data before entering loop
  y = Enes100.getY();
  t = Enes100.getTheta();
  distance = distance1(AtrigPin, AechoPin);

  while (x < 3.4 && y < 1) {  // OSV is outisde goal zone
    x = Enes100.getX();
    y = Enes100.getY();
    t = Enes100.getTheta();
    distance = distance1(AtrigPin, AechoPin);
    
    if (distance > 20 && x < 2.8) { // If no obstacle within 20 cm, but not past all obstacles, face destination then drive
      while (t != 0) {
        right_turn();
        t = Enes100.getTheta(); 
      }
      stop();
      forward();
    } else if (distance <= 20 && x < 2.8) {   // If obstacles ahead (and not past all regular obstacles), do this
      stop();
      if (y < 1) {  // OSV on bottom of arena, want to turn inside
        while (t != PI/2) {
          left_turn();  // Turn from obstacle
          t = Enes100.getTheta();
        }
        stop();
        forward();
        delay(2000);  // Drive "up" for 2 seconds to get around obstacle
        stop();
        while (t != 0) {  
          right_turn(); // Turn back to destination
          t = Enes100.getTheta();
        }
        stop();
      } else if (y >= 1) {  // OSV on top of arena, want to turn inside
        while (t != -PI/2) {
          right_turn();  // Turn from obstacle
          t = Enes100.getTheta();
        }
        stop();
        forward();
        delay(2000);  // Drive "down" for 2 seconds to get around obstacle
        stop();
        while (t != 0) {  
          left_turn(); // Turn back to destination
          t = Enes100.getTheta();
        }
        stop();
      }
    } else if (x >= 2.8) {  // Past all regular obstacles
      while (y < 1.2) {  // If on bottom, we need to get sufficiently into top for limbo
        while (t != PI/2) {
          left_turn();  // Turn upwards to get to limbo
          t = Enes100.getTheta();
        }
        stop();
        forward();
      }
      stop();
      while (t != 0) {
        right_turn();  // Turn back towards right edge of arena (towards limbo)
        t = Enes100.getTheta();
      }
      stop();
      forward();
    }
  }
  stop();

  return 1;
}
