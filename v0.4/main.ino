#include <Arduino.h>
#include "Lights.h"
#include "Animations.cpp"
#include "Effects.h"
#include <time.h>

void setup() {
	Serial.begin(115200);
	Serial.print("Booting...\n");
	SetupLights();
	pinMode(BUTTON_PIN, INPUT);
}

void loop(){
    decorative_bicycle_light();
}


void decorative_bicycle_light(){
    int colormode = 0;
    int brightnessMode = 0;
    double time = 0.0;
    double button_pressed_start = -1;
    double button_pressed_off = -1;
    int longPress = 0;
        
    while(true){
        if(digitalRead(BUTTON_PIN)){
            if(button_pressed_start == -1){
                button_pressed_start = time;
                if(time - button_pressed_off < 0.25){
                    Serial.println("Double press detected!");
                    colormode = 0;
                    brightnessMode = 5;
                }
               
            }
            if(time - button_pressed_start >= 1.0){
                longPress = 1;
            }
        }else if(button_pressed_start != -1){
            double hold_time = time - button_pressed_start;
            button_pressed_start = -1.0;
            button_pressed_off = time;
            longPress = 0;
            if(hold_time > 1.0){
                brightnessMode++;
                brightnessMode %= 6;
                Serial.println("Detected long press, brighnessMode is now "+String(brightnessMode));
            }else{
                colormode++;
                colormode %= 8;
                Serial.println("Colormode is now "+String(colormode));
                if(brightnessMode == 0){
                    brightnessMode = 1;
                    colormode = 0;   
                }
            }
        }
        
        switch(colormode){
            case 0: // white
                SetLed(0, 255,255 ,255 ,255);
                break;
            case 1: // red
                SetLed(0, 255,0 ,0 ,0);
                break;
            case 2: // green
                SetLed(0, 0,255 ,0 ,0);
                break;
            case 3: // blue
                SetLed(0, 0, 0, 255, 0);
                break;
            case 4: // yellow
                SetLed(0, 255, 255, 0, 0);
                break;
            case 5: // pink
                SetLed(0, 255, 0, 255, 0);
                break;
            case 6: // cyan
                SetLed(0, 0, 255, 255, 0);
                break;
            case 7: // rainbow
                int br = (int) (sin(time) * 128);
                Serial.println(br);
                SetLed(0, 128 + br, 128 - br, 0 , 0);
                break;
        }
              
          switch(brightnessMode + longPress){
              case 0: // off
                  DimLed(0, 255,255 ,255 ,255);
                  break;
              case 1: // half power
                  DimLed(0, 128,128 ,128 ,128);
                  break;
              case 2: // full power - don't do anything
                  break;
              case 3: // fade in/fade out
                  {int x = (int) ((1.0 + sin(time)) * 128); 
                  DimLed(0, x, x, x, x);
                  break;}
              case 4: // flashing slow blink
                  {int br = (int) ((sin(time)) * 128); 
                  if(br < 0){
                     DimLed(0, 255, 255, 255, 255);
                  }else{
                     DimLed(0, br, br, br, br);
                  }
                  break;}
               case 5: // slow blink
                    {
                    // -255 --> 255
                    int br = (int) ((sin(time)) * 255); 
                    if(br < 0){
                       DimLed(0, 255, 255, 255, 255);
                    }else{
                       DimLed(0, 255-br, 255-br, 255-br, 255-br);
                    }
                    break;}
          }
        
    
        Commit();
        int awaitMs = 20;
        delay(awaitMs);
        time += 0.001 * awaitMs;
    
    } 

}


void christmas_tree(){
    double time = 0.0;
    BootAnimation bootAnimation;
    bootAnimation.Configure(7.5);
    
    TwinkleLight twinkle[45];
    for(int i = 0; i < 45; i ++){
        int r = 64 * (i % 2 == 0);
        int g = 64 * (i % 3 == 0);
        int b = 64 * (i % 4 == 0);
        int w = 64 * (i % 5 == 0);
        
        twinkle[i].Configure(i, r, g, b, w, 0.5, 0.5,0.5,randto(1.5));
    }
    
    SlowRange travelWhite;
    travelWhite.Configure(32, 32, 10, 128, 10, 30, 0);
     
     SlowRange travelYellow;
     travelYellow.Configure(32, 32, 10, 128, -20, 30, 0);
                             
       
    
    while(true){
        // Clear
        SetRange(0, NUM_LEDS, 0,0,0,0);
          
        for(int i = 0; i < 45; i ++){
           twinkle[i].Animate(time);
        }
        
       // travelYellow.Animate(time);
       // travelWhite.Animate(time);
               
        // bootAnimation.Animate(time);
        
        Commit();
        int awaitMs = 20;
        delay(awaitMs);
        time += 0.001 * awaitMs;
        
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


