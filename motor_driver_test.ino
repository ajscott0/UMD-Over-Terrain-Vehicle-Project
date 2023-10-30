const int left_motor1_pin1 = A1;
const int left_motor1_pin2 = A2;
const int left_motor2_pin1 = A3;
const int left_motor2_pin2 = A4;

void setup() {
  pinMode(left_motor1_pin1, OUTPUT);
  pinMode(left_motor1_pin2, OUTPUT);
  pinMode(left_motor2_pin1, OUTPUT);
  pinMode(left_motor2_pin2, OUTPUT);

  Serial.begin(9600); // Initializing baud rate at 9600 bits/sec.
}

void loop() {
  unsigned long startTime = millis();
  while ((millis() - startTime) < 5000) {
      analogWrite(left_motor1_pin1);
    }

}

// Which pin is to be used can be editted as necessary.