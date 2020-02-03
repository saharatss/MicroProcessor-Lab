#include <Arduino.h>

#define PIN_DATA 8
#define PIN_CLK 10
#define PIN_LATCH 9

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
}

void loop() {
	for(int i=0;i<10;i++){
		setDigit(i);
		delay(500);
	}
}