#include "Enes100.h"

void setup() {
  // Initialize Enes100 Library
  // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
  Enes100.begin("Ban-anna Mike-anics", WATER, 19, 3, 2);
  // At this point we know we are connected.
  Enes100.println("Connected...");
}

void loop() {
  float x, y, t; 
  bool v;
  
  x = Enes100.getX();
  y = Enes100.getY();
  t = Enes100.getTheta();
  v = Enes100.isVisible();

  if (v) {  // If the ArUco marker is visible
    Enes100.print(x); // print out the location
    Enes100.print(",");
    Enes100.print(y);
    Enes100.print(",");
    Enes100.println(t);
    
    /* MS5-specific WiFi communication stuff */
    if (x < 2000) {
      Enes100.print("Our team is on the left side.");
    } else if (x >= 2000) {
      Enes100.print("Our team is on the right side.");
    }

    if (y < 1000) {
      Enes100.print("Our team is on the bottom of the course.");
    } else if (y >= 1000) {
      Enes100.print("Our team is on the top of the course.");
    }

  } else {
    Enes100.println("Not visible"); // print not visible
  }
}