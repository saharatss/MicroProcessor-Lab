#include <Arduino.h>

#define PIN_DATA 8
#define PIN_CLK 9

#define PIN_LATCH1 10
#define PIN_LATCH2 11

#define PIN_BTN1 2
#define PIN_BTN2 3


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

	pinMode(PIN_BTN1,INPUT);
	pinMode(PIN_BTN2,INPUT);
}

int counter = 0;
bool pB1 = false;
bool pB2 = false;

bool directionForward = true;


void loop() {
	bool cB1 = digitalRead(PIN_BTN1);
	bool cB2 = digitalRead(PIN_BTN2);

	if(cB1 && !pB1) directionForward = false;
	if(cB2 && !pB2) directionForward = true;

	if(directionForward) counter ++;
	else counter--;

	if(counter>15) counter=0;
	if(counter<0) counter=15;


	if(counter < 8){
		setLed(PIN_LATCH1, 0x01 << counter);
		setLed(PIN_LATCH2, 0x00);
	}else{
		setLed(PIN_LATCH1, 0x00);
		setLed(PIN_LATCH2, 0x01 << (counter-8));
	}

	pB1 = cB1;
	pB2 = cB2;
	delay(analogRead(2));
}