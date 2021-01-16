#include <Arduino.h>
#include "Lights.h"
#include "Animations.cpp"

void setup() {
	Serial.begin(115200);
	Serial.print("Booting...\n");
	SetupLights();
}


void loopFietskar(){
   double time = 0;
    BootAnimation bootAnimation;
    
    //SlowRange slowRange;
    //slowRange.Configure(0,0,255,0,120,20,0);
    
    SlowRange slowRangeG;
    slowRangeG.Configure(0,128,0,0,120,20,60);
	
    
    double resetTime = bootAnimation.LoopTimeSeconds() * slowRangeG.LoopTimeSeconds();
    
    
    while(true){
	
	
	// Clear
	Effects::SetRange(0, NUM_LEDS, 0,0,0,0);
	
	// Draw a green animation...
	slowRangeG.Animate(time);
	
	// And fade it and the ends
	Effects::DimRangeGradual(8, 0, 255,255,255,255);
	Effects::DimRangeGradual(NUM_LEDS - 8, NUM_LEDS, 255,255,255,255);
	
	// Draw the white front
	Effects::SetRange(0, 8, 0, 0, 0, 255);
	Effects::AddRangeGradual(8,16, 0,0,0,255);
	
	// Draw the red back light
	Effects::SetRange(NUM_LEDS - 8, NUM_LEDS, 255, 0, 0, 0);
	Effects::AddRangeGradual(NUM_LEDS - 8, NUM_LEDS - 16, 255,0,0,0);
	
	
	//slowRange.Animate(time);
	
	
	//Effects::DimRangeGradual(30, 0, 0,255,255,0);
	//Effects::DimRangeGradual(NUM_LEDS - 30, NUM_LEDS-1 , 0,255,255,0);
	//Effects::DimRange(NUM_LEDS - 10, NUM_LEDS , 0,255,255,0);
	
	bootAnimation.Animate(time);
	
	Commit();
	delay(10);
	time += 0.10;
	
	if(time >= resetTime){
	  time = time - resetTime;
	  bootAnimation.Disable();
	}
    }
}


void loop()
{
    double time = 0;
    BootAnimation bootAnimation;
    
    //SlowRange slowRange;
    //slowRange.Configure(0,0,255,0,120,20,0);
    
    SlowRange slowRangeG;
    slowRangeG.Configure(0,128,0,0,120,20,60);
	
    
    double resetTime = bootAnimation.LoopTimeSeconds() * slowRangeG.LoopTimeSeconds();
    
    
    while(true){
	
	
	// Clear
	Effects::SetRange(0, NUM_LEDS, 0,0,0,0);
	
	// Draw a green animation...
	slowRangeG.Animate(time);
	
	// And fade it and the ends
	Effects::DimRangeGradual(8, 0, 255,255,255,255);
	Effects::DimRangeGradual(NUM_LEDS - 8, NUM_LEDS, 255,255,255,255);
	
	// Draw the white front
	Effects::SetRange(0, 8, 0, 0, 0, 255);
	Effects::AddRangeGradual(8,16, 0,0,0,255);
	
	// Draw the red back light
	Effects::SetRange(NUM_LEDS - 8, NUM_LEDS, 255, 0, 0, 0);
	Effects::AddRangeGradual(NUM_LEDS - 8, NUM_LEDS - 16, 255,0,0,0);
	
	
	//slowRange.Animate(time);
	
	
	//Effects::DimRangeGradual(30, 0, 0,255,255,0);
	//Effects::DimRangeGradual(NUM_LEDS - 30, NUM_LEDS-1 , 0,255,255,0);
	//Effects::DimRange(NUM_LEDS - 10, NUM_LEDS , 0,255,255,0);
	
	bootAnimation.Animate(time);
	
	Commit();
	delay(10);
	time += 0.10;
	
	if(time >= resetTime){
	  time = time - resetTime;
	  bootAnimation.Disable();
	}
    }


}


