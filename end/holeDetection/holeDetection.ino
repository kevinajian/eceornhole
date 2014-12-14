  #include <MFRC522.h>
  #include <SPI.h>
  
  #define LED A0
  #define SCORE 8
  
  #define NEN 3 //74HC4067 multiplexor Not Enable signal
  //74HC4067 multiplexor select bits
  #define S0 4
  #define S1 5
  #define S2 6
  #define S3 7
  
  //signals for MFRC22 antenna chip
  #define SS_PIN 10    
  #define RST_PIN 9
  
  #define TAG_LEN 2 //We used NFC PVC RFID Card
                    //The card id's only differed in the 2nd and 3rd bytes for our case so we only save these bytes
  #define ARR_LEN 4 //4 beanbags i.e. cards per team
  
 #define HOLE_ANTENNA 3 //mux channel for hole antenna
 #define BOARD_ANTENNAS 3 //number of board antennas for test case
  
  byte teamA[ARR_LEN][TAG_LEN] = {{0xA8, 0x98}, {0x09, 0x8F}, {0x25, 0x90}, {0xAD, 0x92}}; 
  boolean teamA_check[ARR_LEN];
  
  byte teamB[ARR_LEN][TAG_LEN] = {{0x62, 0x99}, {0x56, 0x8F}, {0x8E, 0x8C}, {0x23, 0x8E}};
  boolean teamB_check[ARR_LEN];
  boolean hole_detection;
  
  
  MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.
  
  int scoreA = 0;
  int scoreB = 0;
  
  
  
  void setup()
  {
          pinMode(NEN, OUTPUT);  
          pinMode(S0, OUTPUT); 
          pinMode(S1, OUTPUT);  
          pinMode(S2, OUTPUT);
          pinMode(S3, OUTPUT);
          digitalWrite(NEN, LOW);
          digitalWrite(S0, HIGH);
          digitalWrite(S1, HIGH);
          digitalWrite(S2, HIGH);
          digitalWrite(S3, HIGH);
          pinMode(SCORE, INPUT);
          digitalWrite(SCORE, HIGH);
          pinMode(LED, OUTPUT);
          digitalWrite(LED, LOW);
          Serial.begin(9600);
          SPI.begin();
          mfrc522.PCD_Init();        // Init MFRC522 card
          init_checker_arrays();
          hole_detection = false;
   }
  
  void init_checker_arrays(){
        for(int i = 0; i < ARR_LEN; i++){
            teamA_check[i] = false;
            teamB_check[i] = false;
          }
  }
  
  void check_and_dump() {
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
          MFRC522::MIFARE_Key key;
          for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
          
          // Look for new cards
          if ( ! mfrc522.PICC_IsNewCardPresent()) {
                  return;
          }
  
          // Select one of the cards
          if ( ! mfrc522.PICC_ReadCardSerial())    return;
          
          byte buffer[2]; //store the 2nd and 3rd byte of the card uid here
          
          //Serial.print("Card UID:");    //Store UID
          for (byte i = 0; i < TAG_LEN; i++) {
            buffer[i] = mfrc522.uid.uidByte[i+1];
            //Debugging to see the right values saved in buffer
            //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
            //Serial.print(buffer[i], HEX);
          } 
         
          update_scores(buffer);
          
          mfrc522.PICC_HaltA(); // Halt PICC
          mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
  }
  
  //Determine the team for the rfid tag, ... 
  //then update score based on whether a tag has already been detected
  void update_scores(byte tag[]){
    
    for(int i = 0; i < ARR_LEN; i++){
      if(!memcmp(tag, teamA[i], TAG_LEN) && !teamA_check[i]){
        if(hole_detection) scoreA += 3;
        else scoreA += 1;
        teamA_check[i] = true;
        break;
      }
      else if(!memcmp(tag, teamB[i], TAG_LEN) && !teamB_check[i]){
        if(hole_detection) scoreB += 3;
        else scoreB += 1;
        teamB_check[i] = true;
        break;
      }
      
    }
    
  }
  
  void activate_and_check_antenna(int antenna_id){//antenna_id correpsonds to the mux channel the antenna is connected to
    int s0 = antenna_id & 0x01;
    int s1 = antenna_id & 0x02;
    int s2 = antenna_id & 0x04;
    int s3 = antenna_id & 0x08;
    digitalWrite(NEN, HIGH);
    digitalWrite(S0, HIGH && s0);
    digitalWrite(S1, HIGH && s1);
    digitalWrite(S2, HIGH && s2);
    digitalWrite(S3, HIGH && s3);
    digitalWrite(NEN, LOW);
    delay(5);
    check_and_dump();
    
  }
  
  
  void check_hole_antenna(){
    hole_detection = true;
    activate_and_check_antenna(HOLE_ANTENNA);
    hole_detection = false;
  }
  
  void check_all_board_antennas() {

    for(int i = 0; i < BOARD_ANTENNAS; i++){
      activate_and_check_antenna(i);
    }
}
  
  
  void loop()
  {
   
    check_hole_antenna();
    // If score button pressed
    if (digitalRead(SCORE)) {
      check_all_board_antennas();
      sendScore(scoreA);
      sendScore(scoreB);
      digitalWrite(LED, HIGH);
      delay(1000);
      digitalWrite(LED, LOW);
      reset();
    }
  }
  
  void sendScore(int score) {
    char score_to_send = char(score) + 'A';
    Serial.print(score_to_send);
  }
  
  void reset() {
    scoreA = 0;
    scoreB = 0;
    init_checker_arrays();
  }
