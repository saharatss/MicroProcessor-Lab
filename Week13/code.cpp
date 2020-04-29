#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,10,9,8,7);

#define BUZZER 13
#define EN_A 2
#define EN_B 3
#define LED1 4
#define LED2 5

class Button{
private:
  bool cState;
  bool pState;
  void (*callback)();
public:
  int pin;
  void begin(int _pin, void (*_callback)()){
    pin = _pin;
    callback = _callback;
    pinMode(pin,INPUT_PULLUP);
  }
  void handle(){
    cState = !digitalRead(pin);
    if(cState && !pState) callback();
    pState = cState;
  }
};

class Thread{
private:
  long int time_start;
  void (*callback)();
public:
  int interval;
  void begin(int _interval, void (*_callback)()){
    interval = _interval;
    callback = _callback;
    time_start = millis();
  }
  void handle(){
    if(millis()-time_start >= interval){
      time_start = millis();
      callback();
    }
  }
};

Button swL;
Button swU;
Button swD;
Button swC;
Button swR;

Thread timer1;
Thread timer2;

bool EN_Ac = false;
bool EN_Bc = false;
bool EN_Ap = false;
bool EN_Bp = false;
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


int 	menu_page = 0;
int   	speed_target;
float 	speed_actual;
int 	tmp_set = 50;
float	tmp_actual;
bool is_protect = false;
bool is_beep = false;

void setSpeed(int speed){
  speed_target = speed;
  speed_target = min(speed_target,100);
  speed_target = max(speed_target,0);
  if(speed==0) speed = 1;
  analogWrite(6,map(speed,0,100,0,180));
}

void pageChange(int _page){
  menu_page = abs(_page%2);
  lcd.clear();
  if(menu_page == 0){
      lcd.setCursor(0, 0);
    lcd.print("Motor Speed(rpm)");
  }else if(menu_page == 1){
    lcd.setCursor(0, 0);
    lcd.print("Protection temp");
  }
}

void sw_pressed(){
}
void swU_callback(){
  sw_pressed();
  if(is_protect)return;
  if(menu_page == 0){
    speed_target+=10;
    setSpeed(speed_target);
  }else if(menu_page == 1){
    tmp_set+=5;
    tmp_set = min(tmp_set,100);
      tmp_set = max(tmp_set,0);
  }
}
void swD_callback(){
  sw_pressed();
  if(is_protect)return;
  if(menu_page == 0){
    speed_target-=10;
    setSpeed(speed_target);
  }else if(menu_page == 1){
    tmp_set-=5;
    tmp_set = min(tmp_set,100);
      tmp_set = max(tmp_set,0);
  }
}
void swL_callback(){
  sw_pressed();
  if(is_protect)return;
  pageChange(--menu_page);
}
void swR_callback(){
  sw_pressed();
  if(is_protect)return;
  pageChange(++menu_page);
}
void swC_callback(){
  sw_pressed();
  if(is_protect){
    is_protect = false;
    is_beep = false;
    pageChange(menu_page);
    setSpeed(speed_target);
  }
}

void timer1_callback(){
  tmp_actual = map(analogRead(A5),522,691,-40,125);
  
  if(tmp_actual > tmp_set){
    is_protect = true;
    is_beep = true;
    tone(BUZZER,50);
  }
  
  if(is_protect){
    digitalWrite(LED1,1);
    digitalWrite(LED2,0);
    lcd.setCursor(0, 0);
      lcd.print(" !!!Protect!!!  ");
    lcd.setCursor(0, 1);
    setSpeed(0);
    if(tmp_actual > tmp_set){
        lcd.print("Temp ");
      lcd.print(int(tmp_actual));
      lcd.print(" > ");
      lcd.print(tmp_set);
      lcd.print("     ");
    }else{
      lcd.print("Press mid button");
    }
    return;
  }else{
    digitalWrite(LED1,0);
    digitalWrite(LED2,1);
  }
  if(!is_beep) noTone(BUZZER);
  
  if(menu_page == 0){
    lcd.setCursor(0, 1);
    lcd.print("set ");
    lcd.print(speed_target);
    lcd.print("  ");
    
    lcd.setCursor(8, 1);
    lcd.print("now ");
    lcd.print(speed_actual);
      lcd.print("  ");
  }else if(menu_page == 1){
    lcd.setCursor(0, 1);
    lcd.print("set ");
    lcd.print(tmp_set);
    lcd.print("  ");
    
    lcd.setCursor(8, 1);
    lcd.print("now ");
    lcd.print(tmp_actual);
      lcd.print("  ");
  }
}

unsigned long int millis_last = 0;
unsigned long int millis_current = 0;
unsigned long int millis_delta = 0;
void timer2_callback(){
  millis_current = millis();
  if(millis_last == 0) millis_last = millis_current;
  millis_delta = millis_current-millis_last;
  speed_actual = ((counter-counterP)/230.0)
    /(millis_delta/1000.0/60.0);
  millis_last = millis_current;
  counterP = counter;
}


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  swL.begin(A0,swL_callback);
  swU.begin(A1,swU_callback);
  swD.begin(A2,swD_callback);
  swC.begin(A3,swC_callback);
  swR.begin(A4,swR_callback);
  
  pinMode(EN_A, INPUT_PULLUP);
  pinMode(EN_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EN_A), EN_Check, FALLING);
  attachInterrupt(digitalPinToInterrupt(EN_B), EN_Check, FALLING);
  
  timer1.begin(10,timer1_callback);
  timer2.begin(100,timer2_callback);
  
  pinMode(BUZZER,OUTPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  
  pageChange(0);
  setSpeed(10);
}

void loop(){
  swL.handle();
  swU.handle();
  swD.handle();
  swC.handle();
  swR.handle();
  timer1.handle();
  timer2.handle();
}