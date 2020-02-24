#include <Arduino.h>

#include <ThreeWire.h>  
#include <RtcDS1302.h>

ThreeWire myWire(4,5,2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

void printDateTime(const RtcDateTime& dt){
	Serial.print(dt.Year(), DEC);
	Serial.print('/');
	Serial.print(dt.Month(), DEC);
	Serial.print('/');
	Serial.print(dt.Day(), DEC);
	Serial.print(' ');
	Serial.print(dt.Hour(), DEC);
	Serial.print(':');
	Serial.print(dt.Minute(), DEC);
	Serial.print(':');
	Serial.print(dt.Second(), DEC);
	Serial.println();
	Serial.print("Total Seconds since 1/1/2000 : ");
	Serial.print(dt.TotalSeconds());
	Serial.println();
}

void setup(){
	Serial.begin(9600);

	Rtc.Begin();

	pinMode(13,OUTPUT);

	RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
	// printDateTime(compiled);
	// Serial.println();
	if(!Rtc.IsDateTimeValid()) {
		Serial.println("RTC lost confidence in the DateTime!");
		Rtc.SetDateTime(compiled);
	}
	if(Rtc.GetIsWriteProtected()){
		Serial.println("RTC was write protected, enabling writing now");
		Rtc.SetIsWriteProtected(false);
	}

	if(!Rtc.GetIsRunning()){
		Serial.println("RTC was not actively running, starting now");
		Rtc.SetIsRunning(true);
	}

	RtcDateTime now = Rtc.GetDateTime();
	if(now < compiled){
		Serial.println("RTC is older than compile time!  (Updating DateTime)");
		Rtc.SetDateTime(compiled);
	}else if(now > compiled){
		Serial.println("RTC is newer than compile time. (this is expected)");
	}else if(now == compiled){
		Serial.println("RTC is the same as compile time! (not expected but all is fine)");
	}
}

bool led_ison = false;
uint32_t previous_TotalSeconds;

void loop(){
	RtcDateTime now = Rtc.GetDateTime();
	// printDateTime(now);

	if(!now.IsValid()) Serial.println("RTC lost confidence in the DateTime!");
	if(previous_TotalSeconds != now.TotalSeconds()){
		led_ison = !led_ison;
		digitalWrite(13,led_ison);
	}

	delay(100);
	previous_TotalSeconds = now.TotalSeconds();
}