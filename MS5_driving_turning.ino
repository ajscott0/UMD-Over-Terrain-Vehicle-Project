#include "Enes100.h"

int turnCount = 0;

// Constant declarations (pins)
const int left_motor1_pin1 = A1;
const int left_motor1_pin2 = A2;
const int left_motor2_pin1 = A3;
const int left_motor2_pin2 = A4;
const int right_motor1_pin1 = A5;
const int right_motor1_pin2 = A6;
const int right_motor2_pin1 = A7;
const int right_motor2_pin2 = A8;

// Function prototypes
void forward();
void backward();
void left_turn();
void right_turn();
void stop();

void setup() {
    pinMode(right_driver_pin1, OUTPUT);
    pinMode(right_driver_pin2, OUTPUT);
    pinMode(left_driver_pin1, OUTPUT);
    pinMode(back_driver_pin2, OUTPUT);

    Serial.begin(9600); // Initializing baud rate at 9600 bits/ssec. Replace with USD sensor's recieving rate
}

void loop() {
    /* MS5 Drive Straight and Turning Code */
    unsigned long startTime = millis();
    while ((millis() - startTime) < 5000) {
      forward();
    }
    stop();

    for (i = 0; i < 3; ++i) { // Need to get this checked. How does order of logic work within this overall loop()??
      left_turn();
      delay(1000);
      stop();
    }
}

// Function implementations
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