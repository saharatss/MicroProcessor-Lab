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

void setLedAll(byte state){
	PORTD = state << 2;
	PORTB = state >> 6;
}

int counter = 0;
bool cB1 = false;
bool cB2 = false;
bool pB1 = false;
bool pB2 = false;

int functionIndex = 0;
int MaxFunctionIndex = 4;

byte currentState = 0x00;

void loop(){
	cB1 = !digitalRead(A0);
	cB2 = !digitalRead(A1);
	
	if(cB1 && !pB1) functionIndex++;
	if(cB2 && !pB2) functionIndex--;
	if(functionIndex>MaxFunctionIndex) functionIndex = 0;
	if(functionIndex<0) functionIndex = MaxFunctionIndex;

	if(functionIndex == 0){
		counter++;
		if(counter>7) counter=0;
		if(counter<0) counter=7;
		currentState = 0x01 << counter;
	}else if(functionIndex == 1){
		counter--;
		if(counter>7) counter=0;
		if(counter<0) counter=7;
		currentState = 0x01 << counter;
	}else if(functionIndex == 2){
		counter++;
		if(counter>15) 	counter=0;
		if(counter<0) 	counter=15;
		if(counter<8) 	currentState = 0x01 << counter;
		else 			currentState = 0x80 >> (counter-8);
	}else if(functionIndex == 3){
		counter++;
		if(counter>3) counter=0;
		switch (counter){
			case 0: currentState = 0b00011000; break;
			case 1: currentState = 0b00100100; break;
			case 2: currentState = 0b01000010; break;
			case 3: currentState = 0b10000001; break;
			default:break;
		}
	}else if(functionIndex == 4){
		counter--;
		if(counter<0) counter=3;
		switch (counter){
			case 0: currentState = 0b00011000; break;
			case 1: currentState = 0b00100100; break;
			case 2: currentState = 0b01000010; break;
			case 3: currentState = 0b10000001; break;
			default:break;
		}
	}else if(functionIndex == 5){
		counter--;
		if(counter>3) counter=0;
		switch (counter){
			case 0: currentState = 0b00011000; break;
			case 1: currentState = 0b00100100; break;
			case 2: currentState = 0b01000010; break;
			case 3: currentState = 0b10000001; break;
			default:break;
		}
	}else{
		functionIndex = 0;
	}
	setLedAll(currentState);

	pB1 = cB1;
	pB2 = cB2;

	delay(analogRead(2));
}