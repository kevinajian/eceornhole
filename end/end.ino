#define IN_HOLEA 2
#define ON_BOARDA 3
#define SCORE 4
#define IN_HOLEB 5
#define ON_BOARDB 6
int scoreA = 0;
int scoreB = 0;

void setup()
{
  pinMode(ON_BOARDA, INPUT);
  digitalWrite(ON_BOARDA, HIGH);
  pinMode(IN_HOLEA, INPUT);
  digitalWrite(IN_HOLEA, HIGH);  
  pinMode(ON_BOARDB, INPUT);
  digitalWrite(ON_BOARDB, HIGH);
  pinMode(IN_HOLEB, INPUT);
  digitalWrite(IN_HOLEB, HIGH);
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
    sendScore(scoreA);
    sendScore(scoreB);
    delay(1000);
    reset();
  }
}

void sendScore(int score) {
  if (score = 0) {
    Serial.print('A');
  } else if (score = 1) {
    Serial.print('B');
  } else if (score = 2) {
    Serial.print('C');
  } else if (score = 3) {
    Serial.print('D');
  } else if (score = 4) {
    Serial.print('E');
  } else if (score = 5) {
    Serial.print('F');
  } else if (score = 6) {
    Serial.print('G');
  } else if (score = 7) {
    Serial.print('H');
  } else if (score = 8) {
    Serial.print('I');
  } else if (score = 9) {
    Serial.print('J');
  } else if (score = 10) {
    Serial.print('K');
  } else if (score = 11) {
    Serial.print('L');
  } else if (score = 12) {
    Serial.print('M');
  }
}

void reset() {
  scoreA = 0;
  scoreB = 0;
}
