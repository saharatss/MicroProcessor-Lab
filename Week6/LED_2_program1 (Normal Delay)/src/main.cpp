#include <Arduino.h>

void setup(){
	Serial.begin(9600);

	pinMode(13,OUTPUT);
	Serial.println("Start");
}

uint32_t totalSeconds = 0;
void loop(){
	totalSeconds++;
	digitalWrite(13,totalSeconds%2);
	if(totalSeconds >= 60*2) Serial.println("Stop");
	delay(1000);
}