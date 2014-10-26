#define IN_HOLE 2
#define ON_BOARD 3
#define SCORE 4
#define TEST_BOARD 5
#define TEST_HOLE 6
#define TEST_SCORE 7
#include <SoftwareSerial.h>
boolean isInHole = false;
boolean isOnBoard = false;
int val = 0;
char code[10];
int bytesread = 0;
#define rxPin 8
#define txPin 9

void setup()
{
  pinMode(ON_BOARD, INPUT);
  digitalWrite(ON_BOARD, HIGH);
  pinMode(IN_HOLE, INPUT);
  digitalWrite(IN_HOLE, HIGH);  
  pinMode(SCORE, INPUT);
  digitalWrite(SCORE, HIGH);
  pinMode(TEST_HOLE, OUTPUT);
  pinMode(TEST_BOARD, OUTPUT);
  pinMode(TEST_SCORE, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  // RFID CODE
  SoftwareSerial RFID = SoftwareSerial(rxPin,txPin); 
  RFID.begin(2400);

  if((val = RFID.read()) == 10)
  {
    isOnBoard = true;
  } 
  
//  //REST OF CODE
  if (!digitalRead(IN_HOLE)) {
    isInHole = true;
  }
  // If score pressed
  if (!digitalRead(SCORE)) {
      digitalWrite(TEST_SCORE, HIGH);
    if (isOnBoard && isInHole) { // Both
      Serial.print('A');
      digitalWrite(TEST_HOLE, HIGH);
      digitalWrite(TEST_BOARD, HIGH);
      isInHole = false;
      isOnBoard = false;
    } else if (isOnBoard) { // on board
      Serial.print('B');
      digitalWrite(TEST_BOARD, HIGH);      
      isOnBoard = false;
    } else if (isInHole) { // in hole
      Serial.print('H');
      digitalWrite(TEST_HOLE, HIGH);      
      isInHole = false;
    }
    delay(1000);
    digitalWrite(TEST_HOLE, LOW);
    digitalWrite(TEST_BOARD, LOW);    
    digitalWrite(TEST_SCORE, LOW);
  }
}
