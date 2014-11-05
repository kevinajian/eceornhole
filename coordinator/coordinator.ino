#define RESET 2;
#define DISPLAY_A 3;
#define DISPLAY_B 4;
#define DISPLAY_1 5;
#define DISPLAY_2 6;
#define DISPLAY_3 7;
#define DISPLAY_4 8;
#define DISPLAY_5 9;
#define DISPLAY_6 10;
#define DISPLAY_7 11;
#define TEST 13;
int incomingByteA;         // a variable to read incoming serial data into
int incomingByteB;         // a variable to read incoming serial data into
int scoreA = 0;
int A10, A1;
int scoreB = 0;
int B10, B1;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(TEST, OUTPUT);
}

void loop() {
// TEST see if there's incoming serial data:
if (Serial.available() > 0) {
  digitalWrite(TEST, HIGH);
}
  if (Serial.available() == 2) { // wait for both scores to come in
    // read the oldest byte in the serial buffer:
    incomingByteA = Serial.read();
//    incomingByteB = Serial.read();
    updateScore();
    // if it's an A (both)
    displayScore();
}

void updateScore() {
  if (incomingByteA == 'B') {
    scoreA += 1;
  } else if (incomingByteA == 'C') {
    scoreA += 2;
  } else if (incomingByteA == 'D') {
    scoreA += 3;
  } else if (incomingByteA == 'E') {
    scoreA += 4;
  } else if (incomingByteA == 'F') {
    scoreA += 5;
  } else if (incomingByteA == 'G') {
    scoreA += 6;
  } else if (incomingByteA == 'H') {
    scoreA += 7;
  }
//  if (incomingByteB == 'B') {
//    scoreA += 1;
//  } else if (incomingByteB == 'C') {
//    scoreA += 2;
//  } else if (incomingByteB == 'D') {
//    scoreA += 3;
//  } else if (incomingByteB == 'E') {
//    scoreA += 4;
//  } else if (incomingByteB == 'F') {
//    scoreA += 5;
//  } else if (incomingByteB == 'G') {
//    scoreA += 6;
//  } else if (incomingByteB == 'H') {
//    scoreA += 7;
//  }
}

void displayScore() {
  A10 = scoreA/10;
  
  displayDigit(A10);
  A1 = scoreA%10;
  displayDigit(A1);
  
}

void reset() {
  scoreA = 0;
//  scoreB = 0;
  // clear serial buffer
  Serial.flush();
digitalWrite(TEST, LOW);
}
