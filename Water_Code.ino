#include "Enes100.h"
#include <stdio.h>

const int trigPin = 9;  // Trigger pin
const int trigPin = 10; // Echo pin

const tdsPin = A0;  // TDS sensor pin
const float referenceVoltage = 5.0;

void setup() {
    // Initialize Enes100 Library
    // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
    Enes100.begin("Ban-anna Mike-anics", WATER, 19, 3, 2);
    // At this point we know we are connected.
    Enes100.println("Connected...");

    Serial.begin(9600); // Initializing baud rate at 9600 bits/ssec. Replace with USD sensor's recieving rate
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
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

    // Code for recieving input from TDS sensor. Probably will go into some time of conditional to know when to run it.
    float tdsAnalogInput = analogRead(tdsPin);
    /* Normalize reading in Arduino analog value range: (0, 2^10) then convert into ppm.
    Conversion may be unneccesary if our own understanding of possible resulting analog values can be determined. */
    float tdsValue = (tdsAnalogInput / 1024.0) * referenceVoltage * 500.0;

    
    // Transmit the state of the pool
    Enes100.mission(WATER_TYPE, FRESH_POLLUTED);
    // Transmit the depth of the pool in mm (20, 30, or 40)
    Enes100.mission(DEPTH, 30);
    delay(1000);
}
