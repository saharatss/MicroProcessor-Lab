#include<Adruino.h>

#define LED1 13
#define SW 2

int mode = 1;
void handle(){
  mode = (mode++)%3+1;
}

void setup(){
  pinMode(LED1, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(SW), handle, RISING);
}

void loop(){
  digitalWrite(LED1, !digitalRead(LED1));
  delay(1000*mode);
}