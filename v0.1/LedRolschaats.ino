#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include <math.h> 

#define LED_PIN     D1
#define NUM_LEDS    30
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB


CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.




void setup() {
	Serial.begin(115200);
	Serial.print("Booting...\n");
	Serial.print(LED_PIN);
    delay( 1000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
}



void loop()
{
	int timeElapsed = 0; // in milliseconds
	int frameDuration = 10; // In milliseconds
	while(true){
		clear();
       
		safety(60);
		
		setLed((timeElapsed / 1000) % 30, 0,128,0);
		
		FastLED.show();
		FastLED.delay(10);
		timeElapsed += 10;
    }
}


inline double soften(double v){
	if(v < 0){
		return 0;
	}
	return sqrt(v);
}


void clear(){
    for(int i = 0; i < NUM_LEDS; i++){
        leds[i] = CRGB(0,0,0);
    }
}

void setLed(int i, int r, int g, int b){
	i = NUM_LEDS - 1 - i; // The led is placed in reverse
	if(i < 0 || i >= NUM_LEDS){
		return;
	}
	leds[i] = CRGB(r, g, b);

}

void safety(int brightness){

	leds[0] = CRGB(brightness, 0,0);
	leds[1] = CRGB(brightness, 0,0);
	leds[2] = CRGB(brightness, 0,0);
	leds[3] = CRGB(brightness, 0,0);
	leds[4] = CRGB(brightness, 0,0);

	leds[25] = CRGB(brightness, brightness,brightness);
	leds[26] = CRGB(brightness, brightness,brightness);
	leds[27] = CRGB(brightness, brightness,brightness);
    leds[28] = CRGB(brightness, brightness,brightness);
    leds[29] = CRGB(brightness, brightness,brightness);

}
