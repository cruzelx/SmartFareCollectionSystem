#include<SoftwareSerial.h>
SoftwareSerial Bluetooth(8,7);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Bluetooth.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
 if(Bluetooth.available()>0){
String temp = Bluetooth.readString();
Bluetooth.println("1234");
Bluetooth.println("kupandole-sundara");
Bluetooth.println("none");
Bluetooth.println(124);
Serial.println(temp);
delay(500);
 }
 if (Serial.available()>0)
 Bluetooth.println(Serial.readString());
}
