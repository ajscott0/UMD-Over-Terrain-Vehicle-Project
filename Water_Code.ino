#include "Enes100.h"

#define POOL_THICK 9 // Sampling pool base is 9mm thick at bottom
#define ARM_HEIGHT 80 // Height of rigid arm in mm

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
const int AtrigPin = 9;  // Trigger pin
const int AechoPin = 10; // Echo pin

const int BtrigPin = 11;  // Trigger pin of USD sensor 2
const int BechoPin = 12;  // Echo pin of USD sensor 2

// Mission sensor pins
const int tdsPin = A0;  // TDS sensor pin
const int turbidityPin = A9;  // Turbidity sensor pin
const int pumpPin1 = A12; // Water pump pin (motor driver pin 1)
const int pumpPin2 = A13; // Water pump pin (motor driver pin 2)

// Function prototypes
void forward();
void backward();
void left_turn();
void right_turn();
void stop();
float distance(int trigPin, int echoPin);
int tds_go();
int turbidity_go();
float depth();
void pump_go();

void setup() {
  // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
  Enes100.begin("Ban-anna Mike-anics", WATER, 19, 3, 2);
  Enes100.println("Connected...");

  pinMode(AtrigPin, OUTPUT);
  pinMode(AechoPin, INPUT);
  pinMode(BtrigPin, OUTPUT);
  pinMode(BechoPin, INPUT);

  pinMode(tdsPin, INPUT);
  pinMode(turbidityPin, INPUT);
  pinMode(pumpPin1, OUTPUT);
  pinMode(pumpPin2, OUTPUT);

  pinMode(rightFrontPin1, OUTPUT);
  pinMode(rightFrontPin2, OUTPUT);
  pinMode(rightRearPin1, OUTPUT);
  pinMode(rightRearPin2, OUTPUT);

  pinMode(leftFrontPin1, OUTPUT);
  pinMode(leftFrontPin2, OUTPUT);
  pinMode(leftRearPin1, OUTPUT);
  pinMode(leftRearPin2, OUTPUT);

  Serial.begin(9600); // Initializing baud rate at 9600 bits/sec. Replace with USD sensor's recieving rate
}

void loop() {
  float x, y, t, distance, water_level; 
  bool v;

  x = Enes100.getX();  // X coordinate: 0-4 meters
  y = Enes100.getY();  // Y coordinate: 0-2 meters
  t = Enes100.getTheta();  // Theta: -pi to +pi in radians
  v = Enes100.isVisible();
  distance = distance(AtrigPin, AechoPin);  // The distance in front of forward facing USD sensor, constantly updated

  if (v) {  // If the ArUco marker is visible
    Enes100.print(x); // print out the location
    Enes100.print(",");
    Enes100.print(y);
    Enes100.print(",");
    Enes100.println(t);
  } else {
    Enes100.println("Not visible");
  }

  /* Navigation Section */
  while (distance )

  
  // **Navigation Section**
  // if (distance_sensor < 20){    //Sense obstacle (20cm) *can change the distance to whatever*
  //     Serial.println("stop");
  //     stop(); // stop (3 seconds)
  //     //We can add a "while (sum > ___) here
  //     //Or just do while instead of if
  // } else { //No obstacle in front
  //     Serial.println("forward");
  //     forward();
  // }

  
  

  
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
