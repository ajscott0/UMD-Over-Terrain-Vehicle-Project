#include "water_code.h"
#include "Enes100.h"
#include <math.h>

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
  float duration, distance_in_func;

  digitalWrite(trigPin, LOW);  // Pulsing trigger pin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);  // Measure time taken for echo pin to go HIGH
  distance_in_func = (duration * 0.0343) / 2;      // Speed of sound = 29 microsec/cm. Divided by 2 b/c back and forth.

  return distance_in_func;
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
  float depth = ARM_HEIGHT - POOL_THICK - 3 - (dist_to_water * 10);  // - 3 from Velcro height

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

void turn_to(double target_angle) {
  int turnDelay = 5;
  bool keep_turning = true;
  double tolerance = 0.15;

  t = Enes100.getTheta();
  while (keep_turning == true) {
    t = Enes100.getTheta();
    if (((t < target_angle + tolerance) && (t > target_angle - tolerance))) {
      stop();
      keep_turning = false;
    } else {
      if ((t - target_angle) < 0) {
        left_turn();
      } else {
        right_turn();
      }
      delay(turnDelay);
      stop();
      delay(turnDelay);
    }
  }
}

int get_to_point(double target_x, double target_y) {
  float cur_x, cur_y, distance;
  double heading, tolerance;
  tolerance = 0.1;

  Enes100.updateLocation();
  cur_x = Enes100.location.x;
  cur_y = Enes100.location.y;
  distance = distance1(AtrigPin, AechoPin);

  heading = atan2(target_y - cur_y, target_x - cur_x);

  turn_to(heading);

  bool keep_driving = true;
  unsigned long current_time;
  unsigned long last_recalculation = 0;
  unsigned long recalculation_interval = 1500;

  while (keep_driving == true) {
    Enes100.updateLocation();
    cur_x = Enes100.location.x;
    cur_y = Enes100.location.y;
    distance = distance1(AtrigPin, AechoPin);
    Enes100.print("Distance: ");
    Enes100.println(distance);

    if (cur_x > (target_x - tolerance) && cur_x < (target_x + tolerance) && cur_y > (target_y - tolerance) && cur_y < (target_y + tolerance)) {
      stop();
      keep_driving = false;
      return 1;
    }
    if (distance < 11) {
      stop();
      keep_driving = false;
      return 2;
    }
    forward();
    current_time = millis();
    if (current_time - last_recalculation >= recalculation_interval) {
      stop();
      heading = atan2(target_y - cur_y, target_x - cur_x);
      turn_to(heading);
      last_recalculation = current_time;
    }
  }

  stop();
}

// Specific Mission Phase Functions
int get_to_site() {
  float cur_x, cur_y, distance;
  Enes100.updateLocation();
  cur_x = Enes100.location.x;
  cur_y = Enes100.location.y;
  distance = distance1(AtrigPin, AechoPin);

  Enes100.println(cur_x);
  Enes100.println(cur_y);

  if (cur_y < 1) {
    get_to_point(0.55, 1.45);
  } else {
    get_to_point(0.55, 0.55);
  }
  stop();
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
  float cur_x, cur_y, distance;
  Enes100.updateLocation();
  cur_x = Enes100.location.x;
  cur_y = Enes100.location.y;
  distance = distance1(AtrigPin, AechoPin);

  if (cur_y > 1) {           // If on top
    get_to_point(1.1, 1.5);  // Approach potential obstacle
    Enes100.println("Function executed");
    if (get_to_point(1.5, 1.5) == 1) {    // Got to point
      get_to_point(1.9, 1.5);             // Continue
    } else {                              // Encountered obstacle
      get_to_point(1.1, 1.0);             // Approach potential obstacle
      if (get_to_point(1.5, 1.0) == 1) {  // Got to point
        get_to_point(1.9, 1.0);           // continue
      } else {                            // Encountered obstacle
        get_to_point(1.1, 0.5);           // Approach known NO obstacle
        get_to_point(1.5, 0.5);           // Got to point
        get_to_point(1.9, 0.5);           // continue
      }
    }
  } else if (y <= 1) {                    // If on bottom
    get_to_point(1.1, 0.5);               // Aproach potential obstalce
    if (get_to_point(1.5, 0.5) == 1) {    // Got to point
      get_to_point(1.9, 0.5);             // Continue
    } else {                              // Encountered obstacle
      get_to_point(1.1, 1.0);             // Approach potential obtacle
      if (get_to_point(1.5, 1.0) == 1) {  // Got to point
        get_to_point(1.9, 1.0);           // continue
      } else {                            // Encountered obstalce
        get_to_point(1.1, 1.5);           // Approach known NO obstacle
        get_to_point(1.5, 1.5);           // Go to point
        get_to_point(1.9, 1.5);           // Continue
      }
    }
  }

  get_to_point(1.9, 1.5);  // Start trying from top either way (closer to limbo)

  if (get_to_point(2.3, 1.5) == 1) {    // Got to point
    get_to_point(3.1, 1.5);             // continue
  } else {                              // Encountered obstacle
    get_to_point(1.9, 1.0);             // Appraoch potential obstalce
    if (get_to_point(2.3, 1.0) == 1) {  // Got to point
      get_to_point(3.1, 1.0);           // continue
    } else {                            // Encountered obstacle
      get_to_point(1.9, 0.5);           // Approach known NO obstacle
      get_to_point(2.3, 0.5);           // Go to point
      get_to_point(3.1, 0.5);           // continue
    }
  }

  get_to_point(3.1, 1.5);  // Go to front of limbo in perperation from going under
  get_to_point(3.7, 1.5);  // Go under limbo

  return 1;
}
