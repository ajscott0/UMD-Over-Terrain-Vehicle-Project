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
const int AtrigPin = 9;  // Trigger pin
const int AechoPin = 10; // Echo pin

const int BtrigPin = 11;  // Trigger pin of USD sensor 2
const int BechoPin = 12;  // Echo pin of USD sensor 2

// Mission sensor pins
const tdsPin = A0;  // TDS sensor pin
const int turbidityPin = A9;  // Turbidity sensor pin
const int pumpPin = A12; // Water pump pin

// Other constant declarations
const float referenceVoltage = 5.0;


// Function prototypes
void forward();
void backward();
void left_turn();
void right_turn();
void stop();
float distance_sensor(int trigPin, int echoPin);
void tds_go();
void turbidity_go();
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
    pinMode(pumpPin, OUTPUT);

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
    float x, y, t; 
    bool v;

    x = Enes100.getX();  // X coordinate: 0-4 meters
    y = Enes100.getY();  // Y coordinate: 0-2 meters
    t = Enes100.getTheta();  // Theta: -pi to +pi in radians
    v = Enes100.isVisible();

    if (v) {  // If the ArUco marker is visible
        Enes100.print(x); // print out the location
        Enes100.print(",");
        Enes100.print(y);
        Enes100.print(",");
        Enes100.println(t);
    } else {
        Enes100.println("Not visible"); // print not visible
    }

    // **Navigation Section**
    if (distance_sensor < 20){    //Sense obstacle (20cm) *can change the distance to whatever*
        Serial.println("stop");
        stop(); // stop (3 seconds)
        //We can add a "while (sum > ___) here
        //Or just do while instead of if
    } else { //No obstacle in front
        Serial.println("forward");
        forward();
    }

    // **Data Collection Section**
    

    
    // Transmit the state of the pool
    Enes100.mission(WATER_TYPE, FRESH_POLLUTED);
    // Transmit the depth of the pool in mm (20, 30, or 40)
    Enes100.mission(DEPTH, 30);
    delay(1000);
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
float distance_sensor(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW); // Pulsing trigger pin
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH); // Measure time taken for echo pin to go HIGH
    int distance = (duration / 29) / 2  // Speed of sound = 29 microsec/cm. Divided by 2 b/c back and forth.

    return distance;
}

void tds_go() {
    float tdsAnalogInput = analogRead(tdsPin);
    // Normalize reading in Arduino analog value range: (0, 1023) then convert into ppm.
    float tdsValue = (tdsAnalogInput / 1024.0) * referenceVoltage * 500.0;

    if (tdsValue <= 450) {  // Fresh
      // Send directly with Enes100.mission
    } else {  // Salty
      // Send directly with Enes100.mission
    }

void turbidity_go() {
    float turbidityValue = analogRead(turbidityPin);

    if (turbidityValue >= 875) {  // Not polluted
      // Send directly with Enes100.mission
    } else {  // Polluted
      // Send directly with Enes100.mission
    }
}

// Turn pump on and run for 10 seconds before stopping
void pump_go() {
    analogWrite(pumpPin, 255);
    delay(10000);
    analogWrite(pumpPin, 0); 
}
