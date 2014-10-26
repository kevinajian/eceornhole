#define IN_HOLE 2
#define ON_BOARD 3
#define SCORE 4

void setup()
{
  pinMode(ON_BOARD, INPUT);
  digitalWrite(ON_BOARD, HIGH);
  pinMode(IN_HOLE, INPUT);
  digitalWrite(IN_HOLE, HIGH);  
  pinMode(SCORE, INPUT);
  digitalWrite(SCORE, HIGH);
  Serial.begin(9600);
}

void loop()
{
  // If score pressed
  if (!digitalRead(SCORE)) {
    if (!digitalRead(ON_BOARD) && !digitalRead(IN_HOLE)) { // Both
      Serial.print('A');
    } else if (!digitalRead(ON_BOARD)) { // on board
      Serial.print('B');
    } else if (!digitalRead(IN_HOLE)) { // in hole
      Serial.print('H');
    }
    delay(1000);
  }
}
