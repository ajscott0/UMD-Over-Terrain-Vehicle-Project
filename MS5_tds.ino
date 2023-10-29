#include "Enes100.h"

const tdsPin = A0;  // TDS sensor pin
float referenceVoltage = 5.0;

float tds_go(); // Function prototype

void setup() {
  pinMode(tdsPin, INPUT);

  Serial.begin(9600); // Initializing baud rate at 9600 bits/ssec. Replace with USD sensor's recieving rate
}

void loop() {
  float tdsValue = tds_go();

  Enes100.print("TDS sensor reading: ");
  Enes100.print(tdsValue);
}

float tds_go() {
    float tdsAnalogInput = analogRead(tdsPin);
    /* Normalize reading in Arduino analog value range: (0, 2^10) then convert into ppm.
    This mapping may be unneccesary if our own understanding of possible resulting analog values can be determined. */
    float tdsValue = (tdsAnalogInput / 1024.0) * referenceVoltage * 500.0;

    return tdsValue;    // Maybe make this boolean value maybe? (salt or no)
}