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

  Serial.begin(9600);  // Initializing baud rate at 9600 bits/sec. Replace with USD sensor's recieving rate
}

void loop() {
  // get_to_site();
  // pump_go();
  // obtain_data();
  get_to_destination();
  delay(1000000);
  // distance = distance1(AtrigPin, AechoPin);
  // Enes100.print("Distance: ");
  // Enes100.println(distance);
  // delay(100);


}
