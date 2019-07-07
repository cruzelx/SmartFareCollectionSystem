#include <LiquidCrystal.h>
const int rs = 6, en = 5, d4 = 4, d5 = 3, d6 = 2, d7 = A0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  
  lcd.begin(16,2);
  Serial.begin(9600);
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("alex");
  lcd.setCursor(0,1);
  lcd.print("bhattarai");
  
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
 
  

}
