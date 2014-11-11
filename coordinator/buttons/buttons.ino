#define A A1
#define B A2

void setup() {
  pinMode(A, INPUT);
  digitalWrite(A, LOW);
  pinMode(B, INPUT);
  digitalWrite(B, LOW);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(A)) {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
  }
  if (digitalRead(B)) {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
  }
}

