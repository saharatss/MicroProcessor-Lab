#include<Arduino.h>
#define MOTOR_D1_PIN 7
#define MOTOR_D2_PIN 8
#define MOTOR_PWM_PIN 9

#define EN_A 2
#define EN_B 3

void setSpeed(int speed){
	if(0<=speed && speed<=255){
		digitalWrite(MOTOR_D1_PIN,1);
		digitalWrite(MOTOR_D2_PIN,0);
		analogWrite(MOTOR_PWM_PIN,speed);
	}else if(-255<=speed && speed<0){
		digitalWrite(MOTOR_D1_PIN,0);
		digitalWrite(MOTOR_D2_PIN,1);
		analogWrite(MOTOR_PWM_PIN,-speed);
	}
}

bool EN_Ac = false; // Current state
bool EN_Bc = false; // Current state
bool EN_Ap = false; // Previous state
bool EN_Bp = false; // Previous state

int counter = 0;

void EN_Handle(bool isCW){ // clockwise 
	if(isCW) counter++;
	else counter--;
	Serial.println("Counter="+String(counter)+" Direction:"+(isCW?"CW":"CCW"));
}

void EN_Check(){
	EN_Ac = !digitalRead(EN_A);
	EN_Bc = !digitalRead(EN_B);
	if(EN_Ac&&EN_Bc){
		if(EN_Ap&&!EN_Bp) EN_Handle(1);
		else if(!EN_Ap&&EN_Bp) EN_Handle(0);
	}
	EN_Ap = EN_Ac;
	EN_Bp = EN_Bc;
}

void setup(){
	pinMode(MOTOR_D1_PIN, OUTPUT);
	pinMode(MOTOR_D2_PIN, OUTPUT);
	pinMode(MOTOR_PWM_PIN, OUTPUT);
	pinMode(EN_A, INPUT_PULLUP);
	pinMode(EN_B, INPUT_PULLUP);
 
	Serial.begin(9600);
	attachInterrupt(digitalPinToInterrupt(EN_A), EN_Check, FALLING);
	attachInterrupt(digitalPinToInterrupt(EN_B), EN_Check, FALLING);
}

void loop(){
	setSpeed(1);
	delay(1000);
	setSpeed(2);
	delay(1000);
	setSpeed(-1);
	delay(1000);
	setSpeed(-2);
	delay(1000);
	
}