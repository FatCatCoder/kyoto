int solenoidPin = 4; // output pin for solenoid control signal

void setup() {
  pinMode(solenoidPin, OUTPUT); // sets as output
}

void loop() {
  digitalWrite(solenoidPin, HIGH); // ON
  delay(45);                       // wait # milliseconds
  digitalWrite(solenoidPin, LOW);  // off
  delay(5000);                     // wait 4 seconds
}
