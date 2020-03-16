# Week8

MicroProcessor-Lab Week 8

## Exercise 1

### Circuit
![alt text](https://github.com/earthsaharat/MicroProcessor-Lab/blob/master/Week8/E1.png "Exercise1")

### Code
```c++
#include <Arduino.h>
#define LED1 10
#define LED2 11

void setup(){
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.begin(9600);
}

String buff="";
int selectedLED = 0;  

void loop(){
  if(Serial.available()>0){
    char inByte = Serial.read();
    buff += inByte;
    if(buff == "L1"){
      selectedLED = LED1;
      buff = "";
    }else if(buff == "L2"){
      selectedLED = LED2;
      buff = "";
    }
    
    if(inByte == 'N'){
      int brightness = float(buff.toInt())*2.54;
      analogWrite(selectedLED,brightness);
      selectedLED = 0;
      buff = "";
    }
  }
}
```

## Exercise 2

### Circuit

![alt text](https://github.com/earthsaharat/MicroProcessor-Lab/blob/master/Week8/E2.png "Exercise2")

### Code
Arduino1 (Lower)
```c++
#include <Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial =  SoftwareSerial(12, 13);

void setup(){
  Serial.begin(9600);
  mySerial.begin(4800);
}

void loop(){
  int value = analogRead(A0);
  mySerial.println(value);
  delay(200);
}
```

Arduino2 (Upper)
```c++
#include <Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial =  SoftwareSerial(13, 12);

void setup(){
  Serial.begin(4800);
  mySerial.begin(4800);
}

String buff = "";
void loop(){
  if(mySerial.available()>0){
    char inByte = mySerial.read();
    if(inByte != '\n'){
      buff += inByte;
    }else{
      int brightness = buff.toInt();
      brightness = map(brightness, 0, 1023, 0, 255);
      Serial.println("brightness = "+String(brightness));
      analogWrite(3,brightness);
      buff = "";
    }
  }
}
```
