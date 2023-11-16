#ifndef WATER_CODE_H
#define WATER_CODE_H

// Macros
#define PI 3.14 // Used in determining angles
#define POOL_THICK 9 // Sampling pool base is 9mm thick at bottom
#define ARM_HEIGHT 80 // Height of rigid arm in mm

// Pin declarations

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

const int BtrigPin = 14;  // Trigger pin of USD sensor 2
const int BechoPin = 15;  // Echo pin of USD sensor 2

// Mission sensor pins
const int tdsPin = A0;  // TDS sensor pin
const int turbidityPin = A9;  // Turbidity sensor pin
const int pumpPin1 = A12; // Water pump pin (motor driver pin 1)
const int pumpPin2 = A13; // Water pump pin (motor driver pin 2)

// Broad-Scope Function Prototypes
void forward();
void backward();
void left_turn();
void right_turn();
void stop();
float distance1(int trigPin, int echoPin);
int tds_go();
int turbidity_go();
float depth();
void pump_go();

// Specific Mission Phase Function Prototypes
int get_to_site();
int collect_water();

#endif
