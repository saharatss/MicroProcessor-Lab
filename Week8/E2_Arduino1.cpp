#include <Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial =  SoftwareSerial(12, 13);

void setup(){
  Serial.begin(9600);
  mySerial.begin(4800);
}

void loop(){
  int value = analogRead(A0);
  mySerial.println(value);
  delay(200);
}