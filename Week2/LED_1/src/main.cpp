#include <Arduino.h>

#define PIN_DATA 8
#define PIN_CLK 9

#define PIN_LATCH1 10
#define PIN_LATCH2 11


void setLed(int latchPin, byte data){
	for(int i=0;i<8;i++){
		digitalWrite(PIN_DATA,data & (0x80>>i));
		digitalWrite(PIN_CLK,1);
		delayMicroseconds(2);
		digitalWrite(PIN_DATA,0);
		digitalWrite(PIN_CLK,0);
		delayMicroseconds(2);
	}
	digitalWrite(latchPin,1);
	delayMicroseconds(2);
	digitalWrite(latchPin,0);
	delayMicroseconds(2);
}

void setup() {
	pinMode(PIN_DATA,OUTPUT);
	pinMode(PIN_CLK,OUTPUT);

	pinMode(PIN_LATCH1,OUTPUT);
	pinMode(PIN_LATCH2,OUTPUT);


	setLed(PIN_LATCH1, 0x00);
	setLed(PIN_LATCH2, 0x00);
}

int counter = 0;
void loop() {
	if(counter < 8){
		setLed(PIN_LATCH1, 0x01 << counter);
		setLed(PIN_LATCH2, 0x00);
	}else{
		setLed(PIN_LATCH1, 0x00);
		setLed(PIN_LATCH2, 0x01 << (counter-8));
	}

	if(counter++ > 15)counter=0;
	delay(100);
}