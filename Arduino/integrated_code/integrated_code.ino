#include <SPI.h>
#include <LiquidCrystal.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

#define RST_PIN         9           
#define SS_PIN          10 

SoftwareSerial Bluetooth(8,7); //For HC-05 bluetooth

//for keypad

const byte ROWS = 1; //1 rows
const byte COLS = 4; //4 columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'}
};
byte rowPins[ROWS] = {A1}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A5, A4, A3, A2}; //connect to the column pinouts of the keypad
+
//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

//for LCD

const int rs = 6, en = 5, d4 = 4, d5 = 3, d6 = 2, d7 = A0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//for RFID

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;
byte deduction;
int serialNumber;
char numberOfPeople;
String fromTo;
char selectRoute;
int remaining;
int paymentMode = 0;
int rechargeMode = 0;
int insufficientMoney = 0;
int rechargedAmount;



void setup() {
  Serial.begin(9600);
  //while(!Serial);
  SPI.begin();
  Bluetooth.begin(9600);
  mfrc522.PCD_Init();    // Init MFRC522

  lcd.begin(16,2);

  for (byte i=0;i<6;i++){
      key.keyByte[i] = 0xFF;
    }

  Serial.println("Choose Mode");
  lcd.setCursor(0,0);
  lcd.print("A: Payment Mode");
  Serial.println("A: Payment Mode");
  lcd.setCursor(0,1);
  lcd.print("B: Recharge Mode");
  Serial.println("B: Recharge Mode");
  //dump_byte_array(key.keyByte,MFRC522::MF_KEY_SIZE);
  Serial.println();

}

void loop() {
    char button = keypad.getKey();
    if (button){
        
        if (button == '1'){
            lcd.clear();
            lcd.print("Payment Mode");
            delay(500);
            lcd.clear();
            lcd.print("Choose Route");
            delay(1500);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("1.Balaju-Sundhara");
            lcd.setCursor(0,1);
            lcd.print("2.Thamel-Balaju");
            delay(1500);
            while(!(selectRoute = keypad.getKey()));
            while(!(selectRoute == '1' || selectRoute == '2')){
                while(!(selectRoute = keypad.getKey()));
              };
            switch(selectRoute){
                case '1':
                fromTo = "Balaju-Sundhara";
                break;
                case '2':
                fromTo = "Thamel-Balaju";
                break;
            }
            lcd.clear();
            lcd.print("Scan the card");

            while (!mfrc522.PICC_IsNewCardPresent());
            if ( ! mfrc522.PICC_ReadCardSerial())
              return;

            MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
     // Serial.println(mfrc522.PICC_GetTypeName(piccType));
  
            if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
                  &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
                  &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
                  //Serial.println(F("This sample only works with MIFARE Classic cards."));
                  return;
              }

            MFRC522::StatusCode status;
        byte buffer[18];
        byte size = sizeof(buffer);
        int AuthAddr = 15;
        int BlockAddr = 12;
        int CheckSector = 3;
         // Authenticate using key A
        status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,AuthAddr , &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("PCD_Authenticate() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
        }
       // Authenticate using key B
        status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, AuthAddr, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("PCD_Authenticate() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
        }

        
         status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(BlockAddr, buffer, &size);
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Read() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
        }
            serialNumber = buffer[0];
            
            
          

          lcd.clear();
          lcd.print("Total People ");
          
          while(! (numberOfPeople = keypad.getKey()));
          if(numberOfPeople != 'A'){
             deduction = 20 * (numberOfPeople-48);
             lcd.clear();
             lcd.setCursor(0,0);
             lcd.print("People: ");
             lcd.print(numberOfPeople);
             lcd.setCursor(0,1);
             lcd.print("Deduction: ");
             lcd.print(deduction);
            }
            else{
                lcd.clear();
                lcd.print("Invalid Input");
              }
              byte MoneyBlockAddr = 8;
        byte MoneySector = 2;
        byte MoneyAuthAddr = 11;
        byte MoneyBlock[16]={0};

        status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, MoneyAuthAddr, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("PCD_Authenticate() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Error Reading");
            lcd.setCursor(0,1);
            lcd.print("Card. Try again");
            return;
        }
        
     
        status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, MoneyAuthAddr, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("PCD_Authenticate() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Error Reading");
            lcd.setCursor(0,1);
            lcd.print("Card. Try again");
            return;
        }
        
        status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(MoneyBlockAddr, buffer, &size);
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Read() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Error Reading");
            lcd.setCursor(0,1);
            lcd.print("Card. Try again");
        }
        if(buffer[0] >=deduction){
        MoneyBlock[0] = buffer[0]-deduction;
        status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(MoneyBlockAddr, MoneyBlock, 16);
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Write() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Error Reading");
            lcd.setCursor(0,1);
            lcd.print("Card. Try again");
        }else{
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Deduced: ");
            lcd.print(deduction);
            Serial.print("Deduced: ");
            Serial.println(deduction);
            lcd.setCursor(0,1);
            lcd.print("Remaining: ");
            lcd.print(buffer[0]-deduction);
            Serial.print("Remaining: ");
            Serial.println(buffer[0]-deduction);
            remaining = buffer[0]-deduction;
            
          }
          }else{
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Insufficient");
            lcd.setCursor(0,1);
            lcd.print("Money");
            Serial.print("Insufficient Money");
            Serial.println();
            insufficientMoney = 1;
            }
            insufficientMoney = 0;
            delay(2000);
            paymentMode = 1;
            rechargeMode = 0;
        
        
        //---------------Deduction End-----------------------------
        // Halt PICC
            mfrc522.PICC_HaltA();
            // Stop encryption on PCD
            mfrc522.PCD_StopCrypto1();
            Serial.println("Choose Mode");
            lcd.setCursor(0,0);
            lcd.print("A: Payment Mode");
            Serial.println("A: Payment Mode");
            lcd.setCursor(0,1);
            lcd.print("B: Recharge Mode");
            Serial.println("B: Recharge Mode");
            //dump_byte_array(key.keyByte,MFRC522::MF_KEY_SIZE);
            Serial.println();
      }else if(button == '2'){

          Serial.println("Chosen Recharge Mode");
          lcd.clear();
          lcd.print("Recharge Mode");
          delay(1500);
          lcd.clear();
          lcd.print("Scan the card");
          MFRC522::StatusCode status;
          byte buffer[18];
          byte size = sizeof(buffer);


        while (!mfrc522.PICC_IsNewCardPresent());
          
  
          
          if ( ! mfrc522.PICC_ReadCardSerial())
              return;
     
    
        
        
       
        MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
       
    
      if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
            &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
            &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
            Serial.println(F("This sample only works with MIFARE Classic cards."));
            return;
        }
          
           
          //----------------------Read Current Balance----------------------

          byte MoneyBlockAddr = 8;
          byte MoneySector = 2;
          byte MoneyAuthAddr = 11;
          byte MoneyBlock[16]={0};

          status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, MoneyAuthAddr, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("PCD_Authenticate() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Error Reading");
            lcd.setCursor(0,1);
            lcd.print("Card. Try again");
            return;
        }
        
     
        status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, MoneyAuthAddr, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("PCD_Authenticate() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Error Reading");
            lcd.setCursor(0,1);
            lcd.print("Card. Try again");
            return;
        }
        status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(MoneyBlockAddr, buffer, &size);
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Read() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Error Reading");
            lcd.setCursor(0,1);
            lcd.print("Card. Try again");
        }
          //----------------------Read Current Balance End------------------

          //----------------------Update Balance Start----------------------

          int recharge = 250;
          Serial.print("Buffer val: ");
          Serial.println(buffer[0]);
          int sum = buffer[0]+ (250 - buffer[0]);
          
          MoneyBlock[0] = sum;
          Serial.println(MoneyBlock[0]);
          if(MoneyBlock[0] >= 255 ){
              Serial.print("Enough Balance");
              lcd.clear();
              lcd.print("Enough Balance");
            }else{

          status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(MoneyBlockAddr, MoneyBlock, 16);
          if (status != MFRC522::STATUS_OK) {
              Serial.print(F("MIFARE_Write() failed: "));
              Serial.println(mfrc522.GetStatusCodeName(status));
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Error Reading");
              lcd.setCursor(0,1);
              lcd.print("Card. Try again");
          }
          lcd.clear();
          lcd.print("Recharged: ");
          lcd.print(250-buffer[0]);
          delay(1000);
          lcd.clear();
          lcd.print("Recharged: ");
          lcd.print(250-buffer[0]);
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Current Balance");
          lcd.setCursor(0,1);
          lcd.print(MoneyBlock[0]);
          delay(1000);
          lcd.clear();
          lcd.print("Return: ");
          lcd.print(buffer[0]);
          Serial.print("Recharged: ");
          Serial.println(250-buffer[0]);
          rechargedAmount = 250-buffer[0];
          remaining = MoneyBlock[0];
          Serial.print("Current Balance: ");
          Serial.println(MoneyBlock[0]);
          Serial.print("Return: ");
          Serial.println(buffer[0]);
          delay(1500);
          rechargeMode = 1;
          paymentMode = 0;
            }
            
            
            
          //----------------------Update Balance End-----------------------
        // Halt PICC
            mfrc522.PICC_HaltA();
            // Stop encryption on PCD
            mfrc522.PCD_StopCrypto1();
            

            Serial.println("Choose Mode12");
            lcd.setCursor(0,0);
            lcd.print("A: Payment Mode");
            Serial.println("A: Payment Mode");
            lcd.setCursor(0,1);
            lcd.print("B: Recharge Mode");
            Serial.println("B: Recharge Mode");
            //dump_byte_array(key.keyByte,MFRC522::MF_KEY_SIZE);
            Serial.println();
          
        }

       //-------------Bluetooth-----------
       if(!(insufficientMoney)){
        while(!(Bluetooth.available()));
        if(paymentMode ==1 && rechargeMode == 0){
             
            String temp = Bluetooth.readString();
            Serial.println(temp);
            Bluetooth.println("payment");
                Bluetooth.println(serialNumber);
                Bluetooth.println(deduction);
                Bluetooth.println(fromTo);
                Bluetooth.println(remaining);
            Serial.println("100000000");  
        }else if(paymentMode == 0 && rechargeMode == 1){
          Bluetooth.println("recharge");
                 Bluetooth.println(serialNumber);
                 Bluetooth.println(rechargedAmount);
                 Bluetooth.println(remaining);
          }
       }
          
}

}


void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
