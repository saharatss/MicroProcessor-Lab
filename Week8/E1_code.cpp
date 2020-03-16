#include <Arduino.h>
#define LED1 10
#define LED2 11

void setup(){
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.begin(9600);
}

String buff="";
int selectedLED = 0;  

void loop(){
  if(Serial.available()>0){
    char inByte = Serial.read();
    buff += inByte;
    if(buff == "L1"){
      selectedLED = LED1;
      buff = "";
    }else if(buff == "L2"){
      selectedLED = LED2;
      buff = "";
    }
    
    if(inByte == 'N'){
      int brightness = float(buff.toInt())*2.54;
      analogWrite(selectedLED,brightness);
      selectedLED = 0;
      buff = "";
    }
  }
}