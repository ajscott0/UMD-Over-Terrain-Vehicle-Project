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

  duration = pulseIn(echoPin, HIGH);           // Measure time taken for echo pin to go HIGH
  distance_in_func = (duration * 0.0343) / 2;  // Speed of sound = 29 microsec/cm. Divided by 2 b/c back and forth.

  return distance_in_func;
}

int tds_go() {
  int dtime = 500;
  int raw = 0;
  int Vin = 5;
  float Vout = 0;
  float R1 = 1000;
  float R2 = 0;
  float buff = 0;
  float avg = 0;
  int samples = 5000;

  float tot = 0;
  for (int i = 0; i < samples; i++) {
    digitalWrite(1, HIGH);
    digitalWrite(0, LOW);
    delayMicroseconds(dtime);
    digitalWrite(1, LOW);
    digitalWrite(0, HIGH);
    delayMicroseconds(dtime);
    raw = analogRead(A3);
    if (raw) {
      buff = raw * Vin;
      Vout = (buff) / 1024.0;
      buff = (Vin / Vout) - 1;
      R2 = R1 * buff;
      //Serial.print("Vout: ");
      //Serial.println(Vout);
      //Serial.print("R2: ");
      //Serial.println(R2);
      tot = tot + R2;
    }
  }

  avg = tot / samples;
  // Serial.print("The average resistance is: ");
  // Enes100.println(avg);
  // Serial.println(" Ohm");
  if (avg > 3000) {
    salt = 1;
  } else {
    salt = 0;
  }
}

int turbidity_go() {
  int valuephoto = analogRead(turbidityPin);

  // Serial.println("Analog Value: ");
  // Serial.println(valuephoto);
  if (valuephoto > 300) {
    polluted = 0;
  } else {
    polluted = 1;
  }
  delay(250);
}

void mission_broadcast() {
  Enes100.mission(DEPTH, depth);

  if (salt == 1 && polluted == 1) {
    Enes100.mission(WATER_TYPE, SALT_POLLUTED);
  } else if (salt == 1 && polluted == 0) {
    Enes100.mission(WATER_TYPE, SALT_UNPOLLUTED);
  } else if (salt == 0 && polluted == 1) {
    Enes100.mission(WATER_TYPE, FRESH_POLLUTED);
  } else {
    Enes100.mission(WATER_TYPE, FRESH_UNPOLLUTED);
  }
}

void depth1() {
  float duration, distance_in_func;

  digitalWrite(BtrigPin, LOW);  // Pulsing trigger pin
  delayMicroseconds(2);
  digitalWrite(BtrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(BtrigPin, LOW);

  duration = pulseIn(BechoPin, HIGH);           // Measure time taken for echo pin to go HIGH
  distance_in_func = (duration * 0.0343) / 2;  // Speed of sound = 29 microsec/cm. Divided by 2 b/c back and forth.

  // Enes100.println(distance_in_func);

  float dist_to_water = distance_in_func;
  depth = 11.4 - (dist_to_water) - 0.9;  // - 3 from Velcro height
  depth = depth * 10;
}

// Turn pump on and run for 10 seconds before stopping
void pump_go() {
  analogWrite(pumpPin2, 0);
  analogWrite(pumpPin1, 255);
  delay(40000);
  analogWrite(pumpPin1, 0);
  analogWrite(pumpPin2, 0);
}

void turn_to(double target_angle) {
  float t;
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

void print_stats() {
  Enes100.print("Distance: ");
  Enes100.println(distance);
  Enes100.print("Current X: ");
  Enes100.println(cur_x);
  Enes100.print("Current Y: ");
  Enes100.println(cur_y);
}

int get_to_point(double target_x, double target_y) {
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
    // print_stats();
    // Enes100.print("Heading: ");
    // Enes100.println(heading);

    if (cur_x > (target_x - tolerance) && cur_x < (target_x + tolerance) && cur_y > (target_y - tolerance) && cur_y < (target_y + tolerance)) {
      stop();
      keep_driving = false;
      return 1;
    }
    if (distance < 15) {
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
  Enes100.updateLocation();
  cur_x = Enes100.location.x;
  cur_y = Enes100.location.y;
  distance = distance1(AtrigPin, AechoPin);

  if (cur_y < 1) {
    double heading, tolerance;
    tolerance = 0.1;

    Enes100.updateLocation();
    cur_x = Enes100.location.x;
    cur_y = Enes100.location.y;
    distance = distance1(AtrigPin, AechoPin);
    float target_x = 0.55;
    float target_y = 1.45;

    heading = atan2(target_y - cur_y, target_x - cur_x);

    turn_to(heading);

    bool keep_driving = true;
    unsigned long current_time;
    unsigned long last_recalculation = 0;
    unsigned long recalculation_interval = 1000;

    while (keep_driving == true) {
      Enes100.updateLocation();
      cur_x = Enes100.location.x;
      cur_y = Enes100.location.y;
      distance = distance1(AtrigPin, AechoPin);
      print_stats();
      Enes100.print("Heading: ");
      Enes100.println(heading);

      if (cur_x > (target_x - tolerance) && cur_x < (target_x + tolerance) && cur_y > (target_y - tolerance) && cur_y < (target_y + tolerance)) {
        stop();
        keep_driving = false;
        return 1;
      }
      if (distance < 3) {
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
  } else {
    double heading, tolerance;
    tolerance = 0.1;

    Enes100.updateLocation();
    cur_x = Enes100.location.x;
    cur_y = Enes100.location.y;
    distance = distance1(AtrigPin, AechoPin);
    float target_x = 0.50;
    float target_y = 0.55;

    heading = atan2(target_y - cur_y, target_x - cur_x);

    turn_to(heading);

    bool keep_driving = true;
    unsigned long current_time;
    unsigned long last_recalculation = 0;
    unsigned long recalculation_interval = 1000;

    while (keep_driving == true) {
      Enes100.updateLocation();
      cur_x = Enes100.location.x;
      cur_y = Enes100.location.y;
      distance = distance1(AtrigPin, AechoPin);
      print_stats();
      Enes100.print("Heading: ");
      Enes100.println(heading);

      if (cur_x > (target_x - tolerance) && cur_x < (target_x + tolerance) && cur_y > (target_y - tolerance) && cur_y < (target_y + tolerance)) {
        stop();
        keep_driving = false;
        return 1;
      }
      if (distance < 3) {
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
  stop();
}

int get_to_destination() {
  Enes100.updateLocation();
  cur_x = Enes100.location.x;
  cur_y = Enes100.location.y;
  // distance = distance1(AtrigPin, AechoPin);

  backward();
  delay(1000);
  stop();

  if (cur_y > 1) {           // If on top
    get_to_point(1.1, 1.5);  // Approach potential obstacle
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
  } else if (cur_y <= 1) {                // If on bottom
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
  turn_to(0);
  forward();
  delay(7000);
  stop();

  // get_to_point(3.7, 1.5);  // Go under limbo

  return 1;
}
