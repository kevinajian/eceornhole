#include <MFRC522.h>
#include <SPI.h>
#define SCORE 13

#define SS_PIN 10    //Arduino Uno
#define RST_PIN 9

#define TAG_LEN 4
#define ARR_LEN 1
#define ARR_LEN2 2

byte teamA[ARR_LEN][TAG_LEN] = 
{{0xA4, 0xF2, 0x97, 0x09A}}; 
//A4 F2 97 9A
//For easy copy/paste/type: {0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x,  0x, 0x, 0x, 0x}

byte teamB[ARR_LEN][TAG_LEN] = 
{{0xB7, 0xD3, 0xEA, 0xBE}};
//0x30, 0x08, 0x04, 0x00, 0x62, 0x63, 0x64, 0x65,  0x66, 0x67, 0x68, 0x69

byte beanbags_detected[ARR_LEN*2][TAG_LEN] = {{0xAA, 0xAA, 0xAA, 0xAA}, {0xBB, 0xBB, 0xBB, 0xBB}};
int num_detected = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.
// CHANGE THESE TO DEFINE AT SOME POINT: VV
int nEn = 3;
int s0 = 4;
int s1 = 5;
int s2 = 6;
int s3 = 7;
// ^^
int scoreA = 0;
int scoreB = 0;

void setup()
{
        pinMode(nEn, OUTPUT);  
        pinMode(s0, OUTPUT); 
        pinMode(s1, OUTPUT);  
        pinMode(s2, OUTPUT);
        pinMode(s3, OUTPUT);
        digitalWrite(nEn, LOW);
        digitalWrite(s0, HIGH);
        digitalWrite(s1, HIGH);
        digitalWrite(s2, HIGH);
        digitalWrite(s3, HIGH);
        mfrc522.PCD_Init();        // Init MFRC522 card
  pinMode(SCORE, INPUT);
  digitalWrite(SCORE, HIGH);
  Serial.begin(9600);
}

void check_and_dump() {
  //mfrc522.PCD_Reset();
  //delay(5);
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
        MFRC522::MIFARE_Key key;
        for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
        
        // Look for new cards
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
                return;
        }

        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial())    return;
        
        Serial.print("Card UID:");    //Dump UID
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
        } 
        Serial.print(" PICC type: ");   // Dump PICC type
        byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
        Serial.println(mfrc522.PICC_GetTypeName(piccType));
         
        byte buffer[18];  
        byte block  = 0;
        byte status;
        //Serial.println("Authenticating using key A...");
        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
           Serial.print("PCD_Authenticate() failed: ");
           Serial.println(mfrc522.GetStatusCodeName(status));
           return;
        }
        
        // Read block
	byte byteCount = sizeof(buffer);
	status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
	if (status != MFRC522::STATUS_OK) {
	    Serial.print("MIFARE_Read() failed: ");
	    Serial.println(mfrc522.GetStatusCodeName(status));
	}
        else  // Dump data
	for (byte index = 0; index < 16; index++) {
	    Serial.print(buffer[index] < 0x10 ? " 0" : " ");
	    Serial.print(buffer[index], HEX);
	    if ((index % 4) == 3) Serial.print(" ");
	}
        Serial.println(" ");
        
        determine_team(buffer);
        
        mfrc522.PICC_HaltA(); // Halt PICC
        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}

void determine_team(byte code[]){
  boolean already_seen = false;
  boolean team_a = false;
  boolean team_b = false;
  for(int i = 0; i < ARR_LEN; i++)
   {
     if(memcmp(code, beanbags_detected[i], TAG_LEN) == 0){
         already_seen = true;
       }
     if(memcmp(code, teamA[i], TAG_LEN) == 0 )
     {
       team_a = true;
       Serial.println("Team A.");
     }
     if(memcmp(code, teamB[i], TAG_LEN) == 0 )
     {
       team_b = true;
       Serial.println("Team B.");
     }
   }
   for(int i = ARR_LEN; i < ARR_LEN2; i++)
   {
     if(memcmp(code, beanbags_detected[i], TAG_LEN) == 0){
         already_seen = true;
       }
   }
   if (!already_seen) {
     Serial.println("We have never seen this tag before!");
     // SEND SCORE HERE!
     for (byte index = 0; index < TAG_LEN; index++) {
	    beanbags_detected[num_detected][index] = code[index];
	}
     num_detected++;
   }
}

void check_all_antennas() {

  //CHCEK ANTENNA 15
  digitalWrite(nEn, LOW);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  delay(5);
  check_and_dump();
  //CHCEK ANTENNA 4
  digitalWrite(nEn, LOW);
  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, HIGH);
  digitalWrite(s3, LOW);
  delay(5);
  check_and_dump();
  //CHCEK ANTENNA 3
  digitalWrite(nEn, LOW);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  delay(5);
  check_and_dump();
  //CHCEK ANTENNA 2
  digitalWrite(nEn, LOW);
  digitalWrite(s0, LOW);
  digitalWrite(s1, HIGH);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  delay(5);
  check_and_dump();
  //CHCEK ANTENNA 1
  digitalWrite(nEn, LOW);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  delay(5);
  check_and_dump();
  //CHECK ANTENNA 0
  digitalWrite(nEn, LOW);
  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  delay(5);
  check_and_dump();
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
    check_all_antennas();
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
