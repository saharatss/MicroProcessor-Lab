#include <Arduino.h>

void setup(){
	DDRD  = 0xFF;
	DDRB  = 0xFF;
}

void setLedOnSingle(int pin){
	if(0 <= pin && pin < 6){
		PORTD = 0x01 << (pin+2);
		PORTB = 0x00;
	}else{
		PORTD = 0x00;
		PORTB = 0x01 << (pin-6);
	}
}

int counter = 0;
bool pB1 = false;
bool pB2 = false;

void loop(){
	bool cB1 = (analogRead(0) == 0);
	bool cB2 = (analogRead(1) == 0);
	
	if(cB1 && !pB1) counter--;
	if(cB2 && !pB2) counter++;

	if(counter>7) counter=0;
	if(counter<0) counter=7;
	setLedOnSingle(counter);

	pB1 = cB1;
	pB2 = cB2;
	delay(100);
}