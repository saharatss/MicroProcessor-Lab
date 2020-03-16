#include <Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial =  SoftwareSerial(13, 12);

void setup(){
  Serial.begin(4800);
  mySerial.begin(4800);
}

String buff = "";
void loop(){
  if(mySerial.available()>0){
    char inByte = mySerial.read();
    if(inByte != '\n'){
      buff += inByte;
    }else{
      int brightness = buff.toInt();
      brightness = map(brightness, 0, 1023, 0, 255);
      Serial.println("brightness = "+String(brightness));
      analogWrite(3,brightness);
      buff = "";
    }
  }
}