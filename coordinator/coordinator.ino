#define DISPLAY_1 2
#define DISPLAY_2 3
#define DISPLAY_3 4
#define DISPLAY_4 5
#define DISPLAY_5 6 
#define DISPLAY_6 7
#define DISPLAY_7 8
#define BUZZER 9
#define DISPLAY_onesA 10
#define DISPLAY_tensB 11
#define DISPLAY_onesB 12
#define WIN_A 13
#define WIN_B A0
#define RESET A1
#define DISPLAY_tensA A2

int incomingByte;         // a variable to read incoming serial data into
int scoreA = 0;
int tempA = 0;
int tensA, onesA;
int scoreB = 0;
int tempB = 0;
int tensB, onesB;
byte index;
int inData[2];

void setup() {
  // initialize I/O
  pinMode(DISPLAY_1, OUTPUT);
  digitalWrite(DISPLAY_1, HIGH);
  pinMode(DISPLAY_2, OUTPUT);
  digitalWrite(DISPLAY_2, HIGH);
  pinMode(DISPLAY_3, OUTPUT);
  digitalWrite(DISPLAY_3, HIGH);
  pinMode(DISPLAY_4, OUTPUT);
  digitalWrite(DISPLAY_4, HIGH);
  pinMode(DISPLAY_5, OUTPUT);
  digitalWrite(DISPLAY_5, HIGH);
  pinMode(DISPLAY_6, OUTPUT);
  digitalWrite(DISPLAY_6, HIGH);
  pinMode(DISPLAY_7, OUTPUT);
  digitalWrite(DISPLAY_7, HIGH);
  pinMode(DISPLAY_tensA, OUTPUT);
  digitalWrite(DISPLAY_tensA, LOW);  
  pinMode(DISPLAY_onesA, OUTPUT);
  digitalWrite(DISPLAY_onesA, LOW);
  pinMode(DISPLAY_tensB, OUTPUT);
  digitalWrite(DISPLAY_tensB, LOW);
  pinMode(DISPLAY_onesB, OUTPUT);
  digitalWrite(DISPLAY_onesB, HIGH);
  pinMode(WIN_A, OUTPUT);
  pinMode(WIN_B, OUTPUT);
  pinMode(RESET, INPUT);
  digitalWrite(RESET, HIGH);
  pinMode(BUZZER, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);

}

void loop() {
  if (!digitalRead(RESET)) {
    quickReset();
  }
  if (Serial.available() > 0) { 
    incomingByte = Serial.read();
    if (incomingByte != 10) {
      inData[index] = incomingByte;
      index++;
    }
  }
  if (index == 2) {  
    if (inData[0] > inData[1]) {
      scoreA += inData[0] - inData[1];
    } else if (inData[1] > inData[0]) {
      scoreB += inData[1] - inData[0];
    }
    index = 0;
  }
  
  displayScore();
  checkWin();
}

void displayScore() {
  tensA = scoreA/10;
  if (tensA != 0) {
    digitalWrite(DISPLAY_tensA, HIGH);
    displayDigit(tensA);
    digitalWrite(DISPLAY_tensA, LOW);  
  }
  onesA = scoreA%10;
  digitalWrite(DISPLAY_onesA, HIGH);
  displayDigit(onesA);
  digitalWrite(DISPLAY_onesA, LOW);
// display B
  tensB = scoreB/10;
  if (tensB != 0) {
    digitalWrite(DISPLAY_tensB, HIGH);
    displayDigit(tensB);
    digitalWrite(DISPLAY_tensB, LOW);
  }
  onesB = scoreB%10;
  digitalWrite(DISPLAY_onesB, HIGH);
  displayDigit(onesB);
  digitalWrite(DISPLAY_onesB, LOW);
}

void displayDigit(int digit) {
  String displayBits = getDisplayBits(digit);
  for (int i=0; i < displayBits.length(); i++) {
    int c = displayBits.charAt(i);
    if (c == '1') {
      int pin = i + 2;
      digitalWrite(pin, LOW);
    }
  }
  for (int i=0; i < displayBits.length(); i++) {
    digitalWrite(i+2, HIGH);
  }
}

String getDisplayBits(int digit) {
  if (digit == 0) {
   return "1111110";
  } else if (digit == 1) {
   return "0110000";
  } else if (digit == 2) {
   return "1101101";
  } else if (digit == 3) {
   return "1111001";
  } else if (digit == 4) {
   return "0110011";
  } else if (digit == 5) {
   return "1011011";
  } else if (digit == 6) {
   return "1011111";
  } else if (digit == 7) {
   return "1110000";
  } else if (digit == 8) {
   return "1111111";
  } else if (digit == 9) {
   return "1110011";
  } 
}

void checkWin() {
  if ((scoreA >= 7 && scoreB == 0) || (scoreA >= 21 && scoreB < 21)) {
    Awin();
  } else if ((scoreB >= 7 && scoreA == 0) || (scoreB >= 21 && scoreA < 21)) {
    Bwin();
  } else if (scoreA >= 21 && scoreB >= 21) {
    if (scoreA > scoreB) {
      Awin();
    } else if (scoreB > scoreA) {
      Bwin();
    }
  }
}

void Awin() {
  digitalWrite(WIN_A, HIGH);
  reset();
}

void Bwin() {
  digitalWrite(WIN_B, HIGH);
  reset();
}

void reset() {
  tone(BUZZER, 50);
  for (int i = 0; i < 5000; i++) {
    displayScore();
  }
  noTone(BUZZER);
  scoreA = 0;
  scoreB = 0;
  digitalWrite(WIN_A, LOW);
  digitalWrite(WIN_B, LOW);
  // clear serial buffer
  Serial.flush();
}

void quickReset() {
  scoreA = 0;
  scoreB = 0;
  Serial.flush();
}  
