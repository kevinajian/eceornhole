const int ledInHole = 12;
const int ledOnBoard = 13; // the pin that the LED is attached to
int incomingByte;         // a variable to read incoming serial data into
boolean displayOn;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledOnBoard, OUTPUT);
  pinMode(ledInHole, OUTPUT);
}

void loop() {
  // see if there's incoming serial data:
  displayOn = false;
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's an A (both)
    if (incomingByte == 'A') {
      digitalWrite(ledInHole, HIGH);
      digitalWrite(ledOnBoard, HIGH);
      displayOn = true;      
    } else if (incomingByte == 'H') { // if it's a capital H (in hole)
      digitalWrite(ledInHole, HIGH);
      displayOn = true;      
    } else if (incomingByte == 'B') { // if it's a capital B (on board)
      digitalWrite(ledOnBoard, HIGH);
      displayOn = true;
    }
  }
  // if a signal was read, clear after 5 seconds
  if (displayOn) {
    delay(3000);
    digitalWrite(ledInHole, LOW);
    digitalWrite(ledOnBoard, LOW);
  }
}
