#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SW1 2
#define SW2 3

#define SCREEN_WIDTH	128
#define SCREEN_HEIGHT	64

#define OLED_RESET		-1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <ThreeWire.h>  
#include <RtcDS1302.h>
ThreeWire myWire(8,9,10); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

String dateTime2String(const RtcDateTime& dt){
	String _buffer = "";
	_buffer += String(dt.Day());
	_buffer += "/" + String(dt.Month());
	_buffer += "/" + String(dt.Year());
	_buffer += " " + String(dt.Hour());
	_buffer += ":" + String(dt.Minute());
	_buffer += ":" + String(dt.Second());
	return _buffer;
}

void setup(){
	pinMode(SW1, INPUT_PULLUP);
	pinMode(SW2, INPUT_PULLUP);

	Rtc.Begin();
	RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
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

	Serial.begin(9600);
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		Serial.println(F("SSD1306 allocation failed"));
		for(;;); // Don't proceed, loop forever
	}
	
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE);

	display.setCursor(0,0);
	display.println(F("SW1"));
	display.display();
}

void loop(){
	display.clearDisplay();
	RtcDateTime now = Rtc.GetDateTime();
	display.setCursor(0,0);
	display.println(dateTime2String(now));
	display.display();
	delay(1000);

}