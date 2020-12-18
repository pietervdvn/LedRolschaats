#include <Arduino.h>
#include <math.h> 
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include "FastLED_RGBW.h"
#include "Properties.h"




#define COLOR_ORDER RGB
#define LED_TYPE    WS2812B

// FastLED with RGBW
CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];

void SetupLights(){
     FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(ledsRGB, getRGBWsize(NUM_LEDS));
     FastLED.setBrightness(  BRIGHTNESS );
     Serial.print("Lights initialized: counts ");
  	 Serial.print(NUM_LEDS);
     Serial.print(" LEds\n");
        	
}

void SetLed(int i, unsigned int r, unsigned int g, unsigned int b, unsigned int w){
	if(i < 0 || i >= NUM_LEDS){
		return;
	}
	leds[i] = CRGBW(r, g, b, w);
}

void AddLed(int i, unsigned int r, unsigned int g, unsigned int b, unsigned int w){
	if(i < 0 || i >= NUM_LEDS){
		return;
	}
	leds[i] += CRGBW(r, g, b, w);
}

void DimLed(int i, unsigned int r, unsigned int g, unsigned int b, unsigned int w){
	if(i < 0 || i >= NUM_LEDS){
		return;
	}
	leds[i] -= CRGBW(r, g, b, w);
}

void Commit(){
    FastLED.show();
}

