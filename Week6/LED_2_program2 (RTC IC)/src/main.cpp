#include <Arduino.h>

#include <ThreeWire.h>  
#include <RtcDS1302.h>

ThreeWire myWire(4,5,2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

String datetime2string(const RtcDateTime& dt){
	return (
		String(dt.Day())+"/"+
		String(dt.Month())+"/"+
		String(dt.Year())+" "+
		String(dt.Hour())+":"+
		String(dt.Minute())+":"+
		String(dt.Second())+" | totalsec:"+
		String(dt.TotalSeconds()));
}

RtcDateTime dt_start;

void setup(){
	Serial.begin(9600);

	pinMode(13,OUTPUT);

	Rtc.Begin();
	dt_start = Rtc.GetDateTime();
	Serial.println("Start");
}

void loop(){
	RtcDateTime now = Rtc.GetDateTime();
	digitalWrite(13,now.Second()%2);
	if(now.TotalSeconds()-dt_start.TotalSeconds() == 60*2) Serial.println("Stop");
}