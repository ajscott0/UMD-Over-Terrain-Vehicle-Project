#include "water_code.h"
#include "Enes100.h"

void setup() {
  // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
  Enes100.begin("Ban-anna Mike-anics", WATER, 19, 10, 11);
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

  // if (v) {  // If the ArUco marker is visible
  //   Enes100.print(x); // print out the location
  //   Enes100.print(",");
  //   Enes100.print(y);
  //   Enes100.print(",");
  //   Enes100.println(t);
  // } else {
  //   Enes100.println("Not visible");
  // }

  /* Navigation Section */
  forward();
  delay(1000);
  stop();
  delay(1000);

  
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
