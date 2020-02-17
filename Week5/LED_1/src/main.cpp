#include <Arduino.h>
#include <SPI.h>

#define DATA_PIN 11
#define CLOCK_PIN 13
#define SS_PIN1 10
#define SS_PIN2 12


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

void setup(){
	pinMode(DATA_PIN, OUTPUT);
	pinMode(CLOCK_PIN, OUTPUT);
	pinMode(SS_PIN1, OUTPUT);
	pinMode(SS_PIN2, OUTPUT);
}

int current_number = 0;

void loop(){
	current_number = (++current_number)%100;
	displayNumber(current_number);
	delay(100);
}