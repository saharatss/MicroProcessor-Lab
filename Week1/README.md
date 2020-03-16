# Week 1

Read/Write digital pin with Register 

![alt text](https://github.com/earthsaharat/MicroProcessor-Lab/blob/master/Week1/IMG_1421.JPG "Arduino")

**X** is port name e.g A,B,C,D

- **DDRX** = pinMode

  ```c++
  // set pin4 of PortD as Input
  DDRD  = 0b00010000;
  ```

- **PORTX** = digitalWrite

  ```c++
  // set status of pin4 of PortD as ON
  PORTD  = 0b00010000;
  ```
  
- **PINX** = digitalRead

  ```c++
  // check status of pin0 of PortB
  if(PINB & 0b00000001){
  }
  ```
