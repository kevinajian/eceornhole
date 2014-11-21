#include <MFRC522.h>
#include <SPI.h>

#define SCORE 8

#define SS_PIN 10    //Arduino Uno
#define RST_PIN 9

#define TAG_LEN 4
#define ARR_LEN 1
#define ARR_LEN2 2

byte teamA[ARR_LEN][TAG_LEN] = {{0xA4, 0xF2, 0x97, 0x09A}}; 
boolean teamA_check[ARR_LEN];
//A4 F2 97 9A
//For easy copy/paste/type: {0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x,  0x, 0x, 0x, 0x}

byte teamB[ARR_LEN][TAG_LEN] = {{0xB7, 0xD3, 0xEA, 0xBE}};
boolean teamB_check[ARR_LEN];
//0x30, 0x08, 0x04, 0x00, 0x62, 0x63, 0x64, 0x65,  0x66, 0x67, 0x68, 0x69

byte beanbags_detected[ARR_LEN*2][TAG_LEN] = {{0xAA, 0xAA, 0xAA, 0xAA}, {0xBB, 0xBB, 0xBB, 0xBB}};
int num_detected = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.

int scoreA = 0;
int scoreB = 0;

void setup()
{
        pinMode(SCORE, INPUT);
        digitalWrite(SCORE, HIGH);
        Serial.begin(9600);
        SPI.begin();
        mfrc522.PCD_Init();        // Init MFRC522 card
        for(int i = 0; i < ARR_LEN; i++){
          teamA_check[i] = false;
          teamB_check[i] = false;
        }
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
        
        /*Serial.print("Card UID:");    //Dump UID
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
        } 
        Serial.print(" PICC type: ");   // Dump PICC type*/
        byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
        //Serial.println(mfrc522.PICC_GetTypeName(piccType));
         
        byte buffer[18];  
        byte block  = 0;
        byte status;
        //Serial.println("Authenticating using key A...");
        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
           //Serial.print("PCD_Authenticate() failed: ");
           //Serial.println(mfrc522.GetStatusCodeName(status));
           return;
        }
        
        // Read block
	byte byteCount = sizeof(buffer);
	status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
	if (status != MFRC522::STATUS_OK) {
	    //Serial.print("MIFARE_Read() failed: ");
	    //Serial.println(mfrc522.GetStatusCodeName(status));
	}
        //else  // Dump data
	/*for (byte index = 0; index < 16; index++) {
	    Serial.print(buffer[index] < 0x10 ? " 0" : " ");
	    Serial.print(buffer[index], HEX);
	    if ((index % 4) == 3) Serial.print(" ");
	}
        Serial.println(" ");*/
        
        update_scores(buffer);
        
        mfrc522.PICC_HaltA(); // Halt PICC
        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}

//Determine the team for the rfid tag, ... 
//then update score based on whether a tag has already been detected
void update_scores(byte tag[]){
  
  for(int i = 0; i < ARR_LEN; i++){
    if(!memcmp(tag, teamA[i], TAG_LEN) && !teamA_check[i]){
      scoreA += 3;
      teamA_check[i] = true;
      break;
    }
    else if(!memcmp(tag, teamB[i], TAG_LEN) && !teamB_check[i]){
      scoreB += 3;
      teamB_check[i] = true;
      break;
    }
    
  }
  
}



void loop()
{
 
  check_and_dump();
  // If score button pressed
  if (digitalRead(SCORE)) {
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
