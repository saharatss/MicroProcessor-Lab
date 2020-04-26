// I have problem about Timer Interrupt in Tinkercad
// So, I use thread instead
// My tinkercad is too slow. Time is running in a milisecond
// This is the best way to control the speed of motor in my case

#define MOTOR_A 7
#define MOTOR_B 8
#define MOTOR_PWM 9
#define EN_A 2
#define EN_B 3

#define LED 13
#define SW1 12
#define SW2 11


int timer1_init = 0;

int speed_target = 1;

void setMotor(int speed){
  speed = map(speed, -100, 100, -180, 180);
  if(0 <= speed){
    digitalWrite(MOTOR_A, 1);
    digitalWrite(MOTOR_B, 0);
  }else{
    speed = -speed;
    digitalWrite(MOTOR_A, 0);
    digitalWrite(MOTOR_B, 1);
  }
  if(speed > 255) speed = 255;
  analogWrite(MOTOR_PWM, speed);
}



bool EN_Ac = false; // Current state
bool EN_Bc = false; // Current state
bool EN_Ap = false; // Previous state
bool EN_Bp = false; // Previous state

int counter = 0;
int counterP = 0;

void EN_Handle(bool isCW){ // clockwise 
  if(isCW) counter--;
  else counter++;
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


int   speed_input = 0;
float speed_calculation = 0;

unsigned long int millis_last = 0;
unsigned long int millis_current = 0;
unsigned long int millis_delta = 0;

int timer_interval = 100;
void timer_handle(){
  millis_current = millis();
  if(millis_last == 0) millis_last = millis_current;
  if(millis_current-millis_last < timer_interval) return;
  millis_delta = millis_current-millis_last;
  
  speed_calculation = -((counter-counterP)/230.0)/(millis_delta/1000.0/60.0);
  
  Serial.print(speed_target);
  Serial.print(",");
  Serial.println(speed_calculation);
  
  millis_last = millis_current;
  counterP = counter;
}

unsigned long timerLED_millis_last = 0;
unsigned long timerLED_millis_current = 0;
void timerLED_handle(){
  timerLED_millis_current = millis();
  if(timerLED_millis_current-timerLED_millis_last < 500) return;
  digitalWrite(LED,!digitalRead(LED));
  timerLED_millis_last = timerLED_millis_current;
}

void setup(){
  pinMode(MOTOR_A, OUTPUT);
  pinMode(MOTOR_B, OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);
  
  pinMode(EN_A, INPUT_PULLUP);
  pinMode(EN_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EN_A), EN_Check, FALLING);
  attachInterrupt(digitalPinToInterrupt(EN_B), EN_Check, FALLING);

  pinMode(LED, OUTPUT);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);

  Serial.begin(9600);
}

bool SW1_c = false;
bool SW1_p = false;
bool SW2_c = false;
bool SW2_p = false;

String serial_buffer = "";
void loop(){
  if(Serial.available()){
    char _key = Serial.read();
    if(_key == 's') serial_buffer = "";
    else if(_key == 'n'){
      speed_target = serial_buffer.toInt();
      speed_target = min(speed_target,100);
      speed_target = max(speed_target,-100);
    }else{
      serial_buffer += String(_key);
    }
  }
  
  SW1_c = !digitalRead(SW1);
  SW2_c = !digitalRead(SW2);
  if(SW1_c && !SW1_p) speed_target = abs(speed_target);
  if(SW2_c && !SW2_p) speed_target = -abs(speed_target);
  SW1_p = SW1_c;
  SW2_p = SW2_c;
  setMotor(speed_target);
  timer_handle();
  timerLED_handle();
}


