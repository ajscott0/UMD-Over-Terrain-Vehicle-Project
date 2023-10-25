#include "Enes100.h"
#include <stdio.h>

const int left_motor1_pin1 = A1;
const int left_motor1_pin2 = A2;
const int left_motor2_pin1 = A3;
const int left_motor2_pin2 = A4;
const int right_motor1_pin1 = A5;
const int right_motor1_pin2 = A6;
const int right_motor2_pin1 = A7;
const int right_motor2_pin2 = A8;

const int trigPin = 9;  // Trigger pin
const int echoPin = 10; // Echo pin 

const tdsPin = A0;  // TDS sensor pin

const float referenceVoltage = 5.0;

void setup() {
    // Initialize Enes100 Library
    // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
    Enes100.begin("Ban-anna Mike-anics", WATER, 19, 3, 2);
    // At this point we know we are connected.
    Enes100.println("Connected...");

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    pinMode(tdsPin, INPUT);

    pinMode(right_driver_pin1, OUTPUT);
    pinMode(right_driver_pin2, OUTPUT);
    pinMode(left_driver_pin1, OUTPUT);
    pinMode(back_driver_pin2, OUTPUT);

    Serial.begin(9600); // Initializing baud rate at 9600 bits/ssec. Replace with USD sensor's recieving rate
}

void loop() {
    float x, y, t; 
    bool v; // Declare variables to hold the data
    //Enes100.getX will make sure you get the latest data available to you about your OTV's location.
    //The first time getX is called, X, Y, theta and visibility are queried and cached.
    //Subsequent calls return from the cache, so there is no performance gain to saving the function response to a variable.

    x = Enes100.getX();  // Your X coordinate! 0-4, in meters, -1 if no aruco is not visibility (but you should use Enes100.isVisible to check that instead)
    y = Enes100.getY();  // Your Y coordinate! 0-2, in meters, also -1 if your aruco is not visible.
    t = Enes100.getTheta();  //Your theta! -pi to +pi, in radians, -1 if your aruco is not visible.
    v = Enes100.isVisible(); // Is your aruco visible? True or False.

    if (v) {  // If the ArUco marker is visible
        Enes100.print(x); // print out the location
        Enes100.print(",");
        Enes100.print(y);
        Enes100.print(",");
        Enes100.println(t);
    } else {
        Enes100.println("Not visible"); // print not visible
    }

    // USD sensor measuring code. Rewrite for each one? - something of the likes
    digitalWrite(trigPin, LOW); // Pulsing trigger pin
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH); // Measure time taken for echo pin to go HIGH
    int distance = (duration / 29) / 2  // Speed of sound = 29 microsec/cm. Divided by 2 b/c back and forth.

    // **Navigation Section**
    if (distance < 20){    //Sense obstacle (20cm) *can change the distance to whatever*
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

void forward() {
    analogWrite(left_motor1_pin1, 200);
    analogWrite(left_motor2_pin1, 200);
    analogWrite(right_motor1_pin1, 200);
    analogWrite(right_motor2_pin1, 200);
}
void backward() {
    analogWrite(left_motor1_pin2, 200);
    analogWrite(left_motor2_pin2, 200);
    analogWrite(right_motor1_pin2, 200);
    analogWrite(right_motor2_pin2, 200);
}
void left_turn() {
    analogWrite(left_motor1_pin2, 200);
    analogWrite(left_motor2_pin2, 200);
    analogWrite(right_motor1_pin1, 200);
    analogWrite(right_motor2_pin1, 200);
}
void right_turn() {
    analogWrite(left_motor1_pin1, 200);
    analogWrite(left_motor2_pin1, 200);
    analogWrite(right_motor1_pin2, 200);
    analogWrite(right_motor2_pin2, 200);
}
void stop() {
    analogWrite(left_motor1_pin1, 0);
    analogWrite(left_motor1_pin2, 0);
    analogWrite(left_motor2_pin1, 0);
    analogWrite(left_motor2_pin2, 0);
    analogWrite(right_motor1_pin1, 0);
    analogWrite(right_motor1_pin2, 0);
    analogWrite(right_motor2_pin1, 0);
    analogWrite(right_motor2_pin2, 0);
}

float tds_go() {
    float tdsAnalogInput = analogRead(tdsPin);
    /* Normalize reading in Arduino analog value range: (0, 2^10) then convert into ppm.
    Conversion may be unneccesary if our own understanding of possible resulting analog values can be determined. */
    float tdsValue = (tdsAnalogInput / 1024.0) * referenceVoltage * 500.0;

    return tdsValue;    // Maybe make this boolean value maybe? (salt or no)
}

float tubidity_go() {
    // code here
}
