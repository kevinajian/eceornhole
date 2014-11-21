#define IN_HOLEA 2
#define ON_BOARDA 3
#define SCORE 4
#define IN_HOLEB 5
#define ON_BOARDB 6
int scoreA = 0;
int scoreB = 0;

void setup()
{
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
  if (digitalRead(SCORE)) {
    // Get count of all on board TODO: ROB
    sendScore(scoreA);
    sendScore(scoreB);
    delay(1000);
    reset();
  }
}

void sendScore(int score) {
  Serial.print(byte(score));
}

void reset() {
  scoreA = 0;
  scoreB = 0;
}
