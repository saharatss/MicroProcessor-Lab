#include<Arduino.h>

#define PIN_DATA 8
#define PIN_CLK 10
#define PIN_LATCH 9

#define DIGIT_1 5
#define DIGIT_2 4
#define DIGIT_3 3

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

int current_number = 1;

void setDigit(int number){
	setLed(NUMBER[number]);
}

void updateDigit(){
	int _delay = 5;
	int digit1 = current_number%10;
	int digit2 = (current_number/10)%10;
	int digit3 = (current_number/100)%10;
	setDigit(digit1);
	digitalWrite(DIGIT_1,1);
	digitalWrite(DIGIT_2,1);
	digitalWrite(DIGIT_3,0);
	delay(_delay);
	setDigit(digit2);
	digitalWrite(DIGIT_1,1);
	digitalWrite(DIGIT_2,0);
	digitalWrite(DIGIT_3,1);
	delay(_delay);
	setDigit(digit3);
	digitalWrite(DIGIT_1,0);
	digitalWrite(DIGIT_2,1);
	digitalWrite(DIGIT_3,1);
	delay(_delay);
}

unsigned long time_start;

void setup() {
	pinMode(PIN_DATA,OUTPUT);
	pinMode(PIN_CLK,OUTPUT);
	pinMode(PIN_LATCH,OUTPUT);

	pinMode(DIGIT_1,OUTPUT);
	pinMode(DIGIT_2,OUTPUT);
	pinMode(DIGIT_3,OUTPUT);

	digitalWrite(DIGIT_1,0);
	digitalWrite(DIGIT_2,0);
	digitalWrite(DIGIT_3,0);

	time_start = millis();
}

void loop() {

	if(millis() - time_start >= 100){
		current_number--;
		time_start = millis();
	}

	if(current_number>999) current_number=0;
	if(current_number<0) current_number=999;

	updateDigit();
}