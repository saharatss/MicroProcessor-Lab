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

void loop(){
	setLedOnSingle(counter);
	counter++;
	counter %= 8;

	delay(1000);
}