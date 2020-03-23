#include<Adruino.h>

#define LED1 13
#define SW 2

int mode = 1;
void handle(){
  mode = (mode++)%3+1;
}

void setup(){
  pinMode(LED1, OUTPUT);

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  
  TCNT1 = 1;
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << TOIE1);
  interrupts();
  
  attachInterrupt(digitalPinToInterrupt(SW), handle, RISING);
}

void loop(){}

int counter = 0;

bool led_state = false;
ISR(TIMER1_OVF_vect){
  TCNT1 = 1;
  if(++counter==mode){
  	led_state = !led_state;
 	digitalWrite(LED1,led_state);
    counter=0;
  }
}