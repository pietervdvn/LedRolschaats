#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include <math.h> 

#define LED_PIN     D1
#define NUM_LEDS    90
#define BRIGHTNESS  50
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];


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
	int frameNumber = 0;
	int frameDuration = 10; // In milliseconds
	while(true){
		clear();
		
		bool reset = animate(frameNumber, frameDuration);
		if(reset){
			frameNumber = 0;
		}
		
		FastLED.show();
		FastLED.delay(25);
		frameNumber ++;
    }
}


uint distance(uint x, uint y){
	// Distance is modular,
	// So if distance is > NUM_LEDS/2, it wraps around
	int d = abs((int) x - (int) y);
	int dWrapAround = NUM_LEDS - d;
	return min(d, dWrapAround);
	
}

void print(char* a, int b){
	Serial.print(a);
	Serial.print(b);
}

void printl(uint i){
	Serial.print("|");
	while(i > 0){
		Serial.print("-");
		i--;
	}
	Serial.print("|");

}

int inrange(int a){
	if(a <= 0){
		return 0;
	}
	if(a >= 100){
		return 100;
	}
	
	return a;
}

bool animate(uint frameNumber, uint frameDurationMillis){

	double rpeak = ( 0 + (25 * frameNumber * frameDurationMillis / 1000)) % NUM_LEDS;
	double gpeak = (30 + (25 * frameNumber * frameDurationMillis / 1000)) % NUM_LEDS;
	double bpeak = (60 + (25 * frameNumber * frameDurationMillis / 1000)) % NUM_LEDS;
	for(uint i = 0; i < NUM_LEDS; i++){
	
		int r = inrange(distance(rpeak, i) * 3 - 45);
		int g = inrange(distance(gpeak, i) * 3 - 45);
		int b = inrange(distance(bpeak, i) * 3 - 45);
		
		
		if(i == 0){
		//	printl(r);
		}
		setLed(i, r, g,b);
	}
	//Serial.print("\n");
	
	return rpeak >= NUM_LEDS;
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


