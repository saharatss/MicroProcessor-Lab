#include<Arduino.h>
#define MOTOR_D1_PIN 7
#define MOTOR_D2_PIN 8
#define MOTOR_PWM_PIN 9

#define EN_A 2
#define EN_B 3

#define DATA_PIN 11
#define CLOCK_PIN 13
#define SS_PIN 10
#define LED1 5
#define LED2 6
#define LED3 4


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
void spi_write(unsigned char data){
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data);
  digitalWrite(SS_PIN,1);
  digitalWrite(SS_PIN,0);
}
int LED_current = 0;
bool LED_isNegative = false;
void setNumber(int number){
  if(number < 0){
    LED_isNegative = true;
    LED_current = -number;
  }else{
    LED_isNegative = false;
    LED_current = number;
  }
  digitalWrite(LED3,LED_isNegative);
}
void LED_display_handle(){
  spi_write(NUMBER[LED_current/10]);
  digitalWrite(LED1,1);
  digitalWrite(LED2,0);
  delay(10);
  spi_write(NUMBER[LED_current%10]);
  digitalWrite(LED1,0);
  digitalWrite(LED2,1);
  delay(10);
  
}

void setSpeed(int speed){
  if(speed>0){
    if(speed>255) speed=255;
    digitalWrite(MOTOR_D1_PIN,1);
  	digitalWrite(MOTOR_D2_PIN,0);
    analogWrite(MOTOR_PWM_PIN,speed);
  }else if(speed<0){
    if(speed<-255) speed=-255;
    digitalWrite(MOTOR_D1_PIN,0);
  	digitalWrite(MOTOR_D2_PIN,1);
    analogWrite(MOTOR_PWM_PIN,-speed);
  }else{ // Fix bug of Tinkercad
    analogWrite(MOTOR_PWM_PIN,1);
  }
}

bool EN_Ac = false; // Current state
bool EN_Bc = false; // Current state
bool EN_Ap = false; // Previous state
bool EN_Bp = false; // Previous state

int target = 0;
int counter = 0;

void EN_Handle(bool isCW){ // clockwise 
  if(isCW) counter++;
  else counter--;
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

void setPosition(int position){
  target = position;
}

float PID_millis_current = 0;
float PID_millis_previous = 0;
float PID_millis_delta = 0;

float PID_error = 0;
float PID_integral = 0;
float PID_derivative = 0;

float PID_previous_error = 0;
float PID_control_value;

float Kp = 5;
float Ki = 0.017;
float Kd = 0.1;

void speedControl_handle(){
  PID_millis_current = millis();
  PID_millis_delta = (PID_millis_current-PID_millis_previous)/1000.0;
  
  PID_error = target-counter;
  PID_integral = PID_integral+PID_error*PID_millis_delta;
  PID_derivative = (PID_error-PID_previous_error)/PID_millis_delta;
  
  PID_control_value = Kp*PID_error + Ki*PID_integral + Kd*PID_derivative;
  setSpeed(PID_control_value);
  PID_previous_error = PID_error;
  PID_millis_previous = PID_millis_current;
}

void setup(){
  pinMode(MOTOR_D1_PIN, OUTPUT);
  pinMode(MOTOR_D2_PIN, OUTPUT);
  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(EN_A, INPUT_PULLUP);
  pinMode(EN_B, INPUT_PULLUP);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(SS_PIN, OUTPUT);
  
 
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(EN_A), EN_Check, FALLING);
  attachInterrupt(digitalPinToInterrupt(EN_B), EN_Check, FALLING);
  setPosition(1);
}

String serialBuffer = "";
void loop(){  
  setPosition(analogRead(A0));
  speedControl_handle();
  setNumber(counter/100);
  LED_display_handle();
  Serial.print(counter);
  Serial.print(",");
  Serial.print(target);
  Serial.println();
}






