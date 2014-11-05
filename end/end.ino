#define IN_HOLEA 2
#define ON_BOARDA 3
#define SCORE 4
#define IN_HOLEB 5
#define ON_BOARDB 6
int scoreA = 0;
int scoreB = 0;

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
  // If in hole, figure out which team, add to score TODO: TALAL
  if (false) {// in hole
    if (false) {// team a
      scoreA += 3;
    } else { // team b
      scoreB += 3;
    }
  }

  // If score pressed
  if (!digitalRead(SCORE)) {
    // Get count of all on board TODO: ROB
    sendScore();
    delay(1000);
    reset();
  }
}

void sendScore() {
  if (scoreA = 0) {
    Serial.print('A');
  } else if (scoreA = 1) {
    Serial.print('B');
  } else if (scoreA = 2) {
    Serial.print('C');
  } else if (scoreA = 3) {
    Serial.print('D');
  } else if (scoreA = 4) {
    Serial.print('E');
  } else if (scoreA = 5) {
    Serial.print('F');
  } else if (scoreA = 6) {
    Serial.print('G');
  } else if (scoreA = 7) {
    Serial.print('H');
  }
  if (scoreB = 0) {
    Serial.print('A');
  } else if (scoreB = 1) {
    Serial.print('B');
  } else if (scoreB = 2) {
    Serial.print('C');
  } else if (scoreB = 3) {
    Serial.print('D');
  } else if (scoreB = 4) {
    Serial.print('E');
  } else if (scoreB = 5) {
    Serial.print('F');
  } else if (scoreB = 6) {
    Serial.print('G');
  } else if (scoreB = 7) {
    Serial.print('H');
  }
}

void reset() {
  scoreA = 0;
  scoreB = 0;
}
