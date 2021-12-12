#include <Arduino.h>
#include "Lights.h"
#include "Animations.cpp"
#include "Effects.h"
#include <time.h>

void setup() {
	Serial.begin(115200);
	Serial.print("Booting...\n");
	SetupLights();
}

void loop(){
    christmas_tree();
}


void christmas_tree(){
    double time = 0.0;
    BootAnimation bootAnimation;
    bootAnimation.Configure(7.5);
    
    TwinkleLight twinkle[50];
    for(int i = 0; i < 50; i ++){
        int r = 64 * (i % 2 == 0);
        int g = 64 * (i % 3 == 0);
        int b = 64 * (i % 4 == 0);
        int w = 64 * (i % 5 == 0);
        
        twinkle[i].Configure(i, r, g, b, w, 0.5, 0.5,0.5,randto(0.5));
    }
       
    double resetTime = 120;
    
    while(true){
        // Clear
        SetRange(0, NUM_LEDS, 0,0,0,0);
          
        for(int i = 0; i < 50; i ++){
            twinkle[i].Animate(time);
        }
        
        Commit();
        int awaitMs = 20;
        delay(awaitMs);
        time += 0.001 * awaitMs;
        
        if(time >= resetTime){
          time = time - resetTime;
          bootAnimation.Disable();
        }
    }

}

void piltover_lantern()
{
    double time = 0;
    BootAnimation bootAnimation;
    bootAnimation.Configure(7.5);
    
    FireAnimation fire;
    fire.Configure(7, 0, 9, 255, 75, 0, 0,  0.5);
    FireAnimation fire_yellow;
    fire_yellow.Configure(7, 0, 6, 162, 162, 0, 0, 1);
    FireAnimation fire_blue;
    fire_blue.Configure(7, 0, 9, 0, 128, 255, 0, 10);
    FireAnimation fire_green;
    fire_green.Configure(7, 0, 7, 50, 255,0, 0, 5);
    
    FlickerAnimation flicker;
    flicker.Configure(20,15,0,32);
    
    
    double resetTime = 120;
    
    while(true){
        // Clear
        SetRange(0, NUM_LEDS, 0,0,0,0);
          
       // SetRange(0, NUM_LEDS, 15,5,0,0); // White background colour
       // AddRange(14, NUM_LEDS-7, 40,20,5,0);
        //SetRange(NUM_LEDS-7, NUM_LEDS, 0,40,60,0); // Blue crown
                      
        
        fire.Animate(time);
        fire_yellow.Animate(time);
        fire_blue.Animate(time);
        flicker.Animate(time);
        bootAnimation.Animate(time);
        
        
        Commit();
        int awaitMs = 20;
        delay(awaitMs);
        time += 0.001 * awaitMs;
        
        if(time >= resetTime){
          time = time - resetTime;
          bootAnimation.Disable();
        }
    }


}


