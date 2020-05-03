#define IC_DATA  10
#define IC_CLK 	 12
#define IC_LATCH 11
#define LED1 9

// == Define my library === 
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
// ======= END =======

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

int speed = 0;

byte LED_display;
byte NUM_display;

int  pattern_index = 0;
int  pattern_counter = 0;
void pattern_handle(){
  switch(pattern_index){
  case 0:
    LED_display = 0x01<<pattern_counter;
    pattern_counter = (++pattern_counter)%8;
  	break;
  case 1:
    LED_display = 0x80>>pattern_counter;
    pattern_counter = (++pattern_counter)%8;
  	break;
  case 2:
    LED_display = (0x80>>pattern_counter) + (1<<pattern_counter);
    pattern_counter = (++pattern_counter)%8;
  	break;
  default:
    pattern_index = 0;
    pattern_handle();
    break;
  }
}

void update_display(){
  shiftOut(IC_DATA,IC_CLK,MSBFIRST,NUMBER[speed+1]);
  shiftOut(IC_DATA,IC_CLK,MSBFIRST,LED_display);
  digitalWrite(IC_LATCH,1);
  digitalWrite(IC_LATCH,0);
}

Button btn1;
Button btn2;
Button btn3;
Thread timer1;

void setup(){
  Serial.begin(9600);
  pinMode(IC_DATA, 	OUTPUT);
  pinMode(IC_CLK, 	OUTPUT);
  pinMode(IC_LATCH, OUTPUT);
  pinMode(LED1, 	OUTPUT);
  btn1.begin(A0,[](){
    speed = (++speed)%5;
  });
  btn2.begin(A1,[](){
    speed = (--speed)%5;
  });
  btn3.begin(A2,[](){
    pattern_index = (++pattern_index)%3;
  });
  timer1.begin(100,[](){
    pattern_handle();
  	update_display();
  });
}

void loop(){
  btn1.handle();
  btn2.handle();
  btn3.handle();
  switch(speed){
  case 0: timer1.interval=1000; break;
  case 1: timer1.interval=500; break;
  case 2: timer1.interval=200; break;
  case 3: timer1.interval=100; break;
  case 4: timer1.interval=50; break;
  }
  timer1.handle();
  delay(10);
}