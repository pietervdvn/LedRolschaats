#include <Arduino.h>
#include "Lights.h"
#include "Properties.h"
#include <math.h>

class Effects {
    public:
        static int modRing(int i){
            while(i < 0){
                i += NUM_LEDS;
            }
            while(i >= NUM_LEDS){
                i -= NUM_LEDS;
            }
            return i;
        }
    
        static void DimRangeGradual(int from, int to, int rDim, int gDim, int bDim, int wDim){
            if(from > to){
             for(int i = to; i < from; i++){
                    double startWeight = ((double) from - i)/(from - to);
                    DimLed(Effects::modRing(i), startWeight * rDim, startWeight * gDim,startWeight * bDim, startWeight * wDim  );
                }
            }else{
                for(int i = from; i < to; i++){
                    double startWeight = 1.0-((double) to - i)/(to - from);
                    DimLed(Effects::modRing(i), startWeight * rDim, startWeight * gDim,startWeight * bDim, startWeight * wDim  );
                
                }
            }
        
        }
        
        static void AddRangeGradual(int from, int to, int rDim, int gDim, int bDim, int wDim){
            if(from > to){
             for(int i = to; i < from; i++){
                    double startWeight = 1.0 - ((double) from - i)/(from - to);
                    AddLed(Effects::modRing(i), startWeight * rDim, startWeight * gDim,startWeight * bDim, startWeight * wDim  );
                }
            }else{
                for(int i = from; i < to; i++){
                    double startWeight = ((double) to - i)/(to - from);
                    AddLed(Effects::modRing(i), startWeight * rDim, startWeight * gDim,startWeight * bDim, startWeight * wDim  );
                
                }
            }
        
        }
        
         static void SetRange(int from, int to, int r, int g, int b, int w){
                for(int i = from; i < to; i++){
                    SetLed(Effects::modRing(i), r, g, b, w  );
                
                }
         }
         
         static void DimRange(int from, int to, int r, int g, int b, int w){
              for(int i = from; i < to; i++){
                  DimLed(Effects::modRing(i), r, g, b, w  );
              
              }
         }
         
        static void AddRange(int from, int to, int r, int g, int b, int w){
            for(int i = from; i < to; i++){
                AddLed(Effects::modRing(i), r, g, b, w  );
               
            }
        }
          static void CountLeds(){
                for(int i = 0; i < NUM_LEDS; i++){
                    SetLed(Effects::modRing(i),
                     (i % 5 == 0) ? 255 : 0,  // Red every 5
                    (i % 2 == 0) ? 255 : 0, // Green every even
                    (i % 25 == 0) ? 255 : 0, // BLue every 25
                    (i % 10 == 0) ? 255 : 0// WHite every 10
                    );
                }
          }

};

class Animation {
    public:
        virtual void Animate(double secondsSinceStart);
        // How long does the animation take
        virtual double LoopTimeSeconds();
};


class TravelingLight: public Animation {
    public:
        void Animate(double secondsSinceStart) {
            secondsSinceStart /= 3;
            int lowLed = floor(secondsSinceStart);
            int highLed = lowLed + 1;
            double highLedBrightness = (secondsSinceStart - lowLed);
            
            AddLed(lowLed, 0, 0,0,(1 - highLedBrightness) * 255);
            AddLed(highLed % NUM_LEDS, 0,0,0,highLedBrightness * 255);
        }
        
        double LoopTimeSeconds(){
            return NUM_LEDS * 3;
        }
};


class SlowRange: public Animation {
 private:
    int r, g, b, w, width;
    double cycleTime, phaseShift; // in seconds
   
 public:
        void Configure(int r, int g, int b, int w, double cycleTime, int width, double phaseShift){
            this->r = r;
            this->g = g;
            this->b = b;
            this->w = w;
            this->width = width;
            this->cycleTime = cycleTime;
            this->phaseShift = phaseShift;
        }
    
        void Animate(double secondsSinceStart) {
            secondsSinceStart += phaseShift;
            double start = NUM_LEDS * (secondsSinceStart / this->cycleTime);
            for(int i = floor(start); i < floor(start) + width + 1; i ++){
                double intensity = start + (width / 2.0);
                    intensity -= i;
                if(intensity < 0){
                    intensity = -intensity;
                }
                intensity /= width / 2.0;
                intensity = 1.0 - intensity;
                if(intensity < 0){
                    intensity = 0;
                }
                
                AddLed(Effects::modRing(i), this->r * intensity, this->g * intensity, this->b * intensity, this->w * intensity);
            }
            
        
        }
        
         double LoopTimeSeconds(){
             return this->cycleTime;
         }

};

// This animations first dims all leds, then allows the middle leds to turn on until all the leds are enabled
// THere is a window of 25% of the leds where there is a partial gradient
class BootAnimation: public Animation {
    private:
        bool enabled = true;
    public:
        void Disable(){
            this->enabled = false;
        }
    
    
        void Animate(double secondsSinceStart) {
           if(!this->enabled){
                return;
           }
           
           secondsSinceStart = (secondsSinceStart) * NUM_LEDS / 30.0;
           
           double center = NUM_LEDS / 2.0;
           double slopeLength = NUM_LEDS / 4;
           double cutoffPoint = (center - secondsSinceStart - 1);
           for(int i = 0; i < max(0.0, cutoffPoint); i++){
                double d = (cutoffPoint - i); // Distance to the cutoffPoint, in nr of leds
                DimLed(i, 255 ,255 ,255,255);
                DimLed(NUM_LEDS - i, 255,255,255,255);
           }
           
            for(int i = min(0.0, cutoffPoint); i < min(center, cutoffPoint + slopeLength); i++){
                double d = 1.0 - (i - cutoffPoint) / slopeLength; 
                DimLed(i, d * 255 , d * 255 ,d * 255, d * 255);
                DimLed(NUM_LEDS - i, d * 255,d * 255,d * 255,d * 255);
            }
           
           
        }
        
        double LoopTimeSeconds(){
            return 30;
        }
};