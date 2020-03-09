#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SW1 2
#define SW2 3

#define SCREEN_WIDTH	128 // OLED display width, in pixels
#define SCREEN_HEIGHT	64 // OLED display height, in pixels

#define OLED_RESET		-1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup(){
	pinMode(SW1, INPUT_PULLUP);
	pinMode(SW2, INPUT_PULLUP);

	Serial.begin(9600);
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
		Serial.println(F("SSD1306 allocation failed"));
		for(;;); // Don't proceed, loop forever
	}
	
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE);

	display.setCursor(0,0);
	display.println(F("SW1"));
	display.drawCircle(5, 15, 5, SSD1306_WHITE);

	display.setCursor(0,30);
	display.println(F("SW2"));
	display.drawCircle(5, 45, 5, SSD1306_WHITE);

	display.display();
}

void loop(){
	if(!digitalRead(SW1)) 	display.fillCircle(5, 15, 5, SSD1306_WHITE);
	else 					display.fillCircle(5, 15, 5, SSD1306_BLACK);
	display.drawCircle(5, 15, 5, SSD1306_WHITE);

	if(!digitalRead(SW2)) 	display.fillCircle(5, 45, 5, SSD1306_WHITE);
	else 					display.fillCircle(5, 45, 5, SSD1306_BLACK);
	display.drawCircle(5, 45, 5, SSD1306_WHITE);
	display.display();
}