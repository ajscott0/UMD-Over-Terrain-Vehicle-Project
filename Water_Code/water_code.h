#ifndef WATER_CODE_H
#define WATER_CODE_H

// Macros
#define PI 3.14 // Used in determining angles
#define POOL_THICK 9 // Sampling pool base is 9mm thick at bottom
#define ARM_HEIGHT 110 // Height of rigid arm in mm

// Pin declarations
// const int salin1 = 0;
// const int salin2 = 1;

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

const int BtrigPin = A4;  // Trigger pin of USD sensor 2
const int BechoPin = A3;  // Echo pin of USD sensor 2

// Mission sensor pins
const int tdsPin = A0;  // TDS sensor pin
const int turbidityPin = A3;  // Turbidity sensor pin
const int pumpPin1 = A1; // Water pump pin (motor driver pin 1)
const int pumpPin2 = A2; // Water pump pin (motor driver pin 2)

float cur_x, cur_y, distance;
int salt = 0, polluted = 0;
float depth = 30;

// Broad-Scope Function Prototypes
void forward();
void backward();
void left_turn();
void right_turn();
void stop();
float distance1(int trigPin, int echoPin);
int tds_go();
int turbidity_go();
void mission_broadcast();
void depth1();
void pump_go();
void print_stats();

void turn_to(double target_angle);
int get_to_point(double target_x, double target_y);

// Specific Mission Phase Function Prototypes
int get_to_site();
int obtain_data();
int get_to_destination();

#endif
