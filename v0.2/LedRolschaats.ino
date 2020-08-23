#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include <math.h> 

#define LED_PIN     D4
#define NUM_LEDS    120
#define BRIGHTNESS  50
#define LED_TYPE    WS2813
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
	
	int buttonState = 0;
	while(true){
		clear();
		bool reset = animate(frameNumber, frameDuration);
		if(reset){
			frameNumber = 0;
		}
		
		FastLED.show();
		frameNumber ++;
		FastLED.delay(25);
    }
}


double distance(double x, double y){
	// Distance is modular,
	// So if distance is > NUM_LEDS/2, it wraps around
	double d = x - y;
	if(d < 0){
		d = -d;
	}
	double dWrapAround = NUM_LEDS - d;
	return min(d, dWrapAround);
	
}

void print(char* a, int b){
	Serial.print(a);
	Serial.print(b);
	Serial.print(" ");
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

double mod(double d, double operand){
	while(d < 0){
		d += operand;
	}
	while(d > operand){
		d -= operand;
	}
	return d;

}


inline double modring(double d){
	return mod(d, NUM_LEDS);

}

inline double modone(double d){
	return mod(d, 1);
}

inline double dabs(double d){
	if(d < 0){
		return -d;
	}
	return d;
}



bool animate(uint frameNumber, uint frameDurationMillis){

	double loopDuration = 30; // seconds for the entire loop
	double loopRatio = 1.0 * frameNumber * frameDurationMillis / (1000 *  loopDuration);

	double rpeak = modone( loopRatio * 11 ) * NUM_LEDS;
	double rintensity = dabs(0.5 - modone(loopRatio * 22)) * 2 ;

	double gpeak = modone( loopRatio * -7 + 1/3) * NUM_LEDS;
	double gintensity = dabs(0.5 - modone(loopRatio * 14)) * 2 ;

	
	double bpeak = modone(loopRatio * 5 + 2 / 3) * NUM_LEDS;
	double bintensity = dabs(0.5 - modone(loopRatio * 10)) * 2 ;

	double ypeak = modone(loopRatio * -3 + 1 / 3) * NUM_LEDS;
	double yintensity = dabs(0.5 - modone(loopRatio * 6)) * 2 ;

	for(uint i = 0; i < NUM_LEDS; i++){
		// Distance: value between 0 and 45 (or: NUM_LEDS/2)
		double r = inrange(distance(rpeak, i) * 4 - 180 + 5 + rintensity * 30)
			+ inrange(distance(ypeak, i) * 4 - 180 + yintensity *30);
		double g = inrange(distance(gpeak, i) * 4 - 180 + 5 + gintensity * 30)
			+ inrange(distance(ypeak, i) * 4 - 180 + yintensity * 50);
		int b = inrange(distance(bpeak, i) * 4 - 180 + bintensity * 30);
		
		setLed(i, r, g , b);
	}//*/
	Serial.print("\n");
	
	return loopRatio >= 1;
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


