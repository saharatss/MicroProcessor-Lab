#include <Arduino.h>
#include <SPI.h>

#define DATA_PIN 11
#define CLOCK_PIN 13
#define SS_PIN1 10
#define SS_PIN2 12

#define PIN_BTN1 3
#define PIN_BTN2 2
#define PIN_BTN3 4

#define PIN_BUZZER 7

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

void spi_write(unsigned char data,int ss_pin){
	shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data);
	digitalWrite(ss_pin,1);
	digitalWrite(ss_pin,0);
}
void displayNumber(int number){
	spi_write(NUMBER[number%10],SS_PIN1);
	spi_write(NUMBER[number/10],SS_PIN2);
}

void setup()
{
	pinMode(DATA_PIN, OUTPUT);
	pinMode(CLOCK_PIN, OUTPUT);
	pinMode(SS_PIN1, OUTPUT);
	pinMode(SS_PIN2, OUTPUT);

	pinMode(PIN_BUZZER, OUTPUT);
	
	pinMode(PIN_BTN1,INPUT);
	pinMode(PIN_BTN2,INPUT);
}

int counter = 0;
int counterms = 0;
bool isCountDown = false;
bool isAlarm = false;

bool pB1 = false;
bool pB2 = false;
bool pB3 = false;

void loop(){  
	bool cB1 = digitalRead(PIN_BTN1);
	bool cB2 = digitalRead(PIN_BTN2);
	bool cB3 = digitalRead(PIN_BTN3);
	
	if(isCountDown){
		counterms--;
		if(counterms<=0){
			counterms = 1000;
			counter--;
			displayNumber(counter);
		}
		if(counter==0){
			isCountDown=false;
			isAlarm=true;
			tone(PIN_BUZZER, 1000);
		}
	}else{
		if(cB1 && !pB1) counter--;
		if(cB2 && !pB2) counter++;
		displayNumber(counter);
	}

	if(cB3 && !pB3){
		if(isAlarm){
			noTone(PIN_BUZZER);
			isAlarm=false;
		}else{
			isCountDown=!isCountDown;
		}
	}
		
	if(counter>99) counter=0;
	if(counter<0) counter=99;
		
	pB1 = cB1;
	pB2 = cB2;
	pB3 = cB3;
	delay(1);
}