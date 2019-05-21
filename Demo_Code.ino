//this is demostration code for ht

#include "segmentLcd.h"

segmentLcd lcd;

void setup(){
  lcd.begin(7,6,5);
}

void loop(){
  lcd.print(1234);
  delay (3000);
  lcd.printFloat(3.14);
  delay (3000);
  lcd.printTime(02,13);
  delay (3000);
  lcd.printC(33.4);
  delay (3000);
  for (int i =0; i<=10;i++){
    lcd.print(i);
    delay(1000);
  }
}
