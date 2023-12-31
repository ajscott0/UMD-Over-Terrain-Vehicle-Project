#include "water_code.h"
#include "Enes100.h"
#include <math.h>

void setup() {
  // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
  delay(1000);
  Enes100.begin("Ban-anna Mike-anics", WATER, 10, 11, 10);
  Enes100.println("Connected...");
  delay(2000);

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

  Serial.begin(9600);  // Initializing baud rate at 9600 bits/sec.
}

void loop() {
  get_to_site();
  delay(5000);
  pump_go();
  turbidity_go();
  tds_go();
  depth1();
  mission_broadcast();
  get_to_destination();

  // Reverse pump code: drain water collection dish
  // analogWrite(pumpPin2, 255);
  // analogWrite(pumpPin1, 0);
  // delay(40000);
  // analogWrite(pumpPin1, 0);
  // analogWrite(pumpPin2, 0);

  delay(1000000);
}
