# Week 6

Countdown 2 minutes with 1 led toggle every second

Normal Delay Function
-> 1minute 59.23seconds

RTC IC 
-> 2minutes 0seconds

RTC IC is more accurate than using normal delay function of Arduino. Because Arduino is a single core of processer and every commands are running sequentially (one by one, top to bottom of codes). If we use delay for 1000ms and assume that the command will delay for real 1 second but the code did not has just delay command. The code has other command that running (LED toggle) and they are using a few millisecond for running. Therefore 1 loop will not exactly one second. But RTC IC was design for running only timer function or time counter function that make the IC more accurate. That why 2 minutes of normal delay function is not equal to RTC IC.
