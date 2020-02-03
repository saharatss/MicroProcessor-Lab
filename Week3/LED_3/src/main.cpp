#include <Arduino.h>

#define PIN_DATA 8
#define PIN_CLK 10
#define PIN_LATCH 9

#define BTN1 6
#define BTN2 7

byte NUMBER[] = {
	0b00111111,
	0b00000110,
	0b01011011,
	0b01001111,
	0b01100110,
	0b01101101,
	0b01111101,
	0b00000111,
	0b01111111,
	0b01101111
};

void setLed(byte data){
	for(int i=0;i<8;i++){
		digitalWrite(PIN_DATA,data & (0x80>>i));
		digitalWrite(PIN_CLK,1);
		delayMicroseconds(2);
		digitalWrite(PIN_DATA,0);
		digitalWrite(PIN_CLK,0);
		delayMicroseconds(2);
	}
	digitalWrite(PIN_LATCH,1);
	delayMicroseconds(2);
	digitalWrite(PIN_LATCH,0);
	delayMicroseconds(2);
}

void setDigit(int number){
	setLed(NUMBER[number]);
}

void setup() {
	pinMode(PIN_DATA,OUTPUT);
	pinMode(PIN_CLK,OUTPUT);
	pinMode(PIN_LATCH,OUTPUT);

	pinMode(BTN1,INPUT);
	pinMode(BTN2,INPUT);
}

int counter = 0;

bool pB1 = false;
bool pB2 = false;

void loop() {
	bool cB1 = digitalRead(BTN1);
	bool cB2 = digitalRead(BTN2);

	if(cB1 && !pB1) counter++;
	if(cB2 && !pB2) counter--;
	
	if(counter>9) counter=0;
	if(counter<0) counter=9;

	setDigit(counter);

	pB1 = cB1;
	pB2 = cB2;
}