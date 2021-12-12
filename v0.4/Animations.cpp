#include <Arduino.h>
#include "Lights.h"
#include "Properties.h"
#include <math.h>
#include "Effects.h"

class Animation {
    public:
        virtual void Animate(double secondsSinceStart);
        // How long does the animation take
        virtual double LoopTimeSeconds();
};

// A single led travelling all leds, ideal to test the leds
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

// SlowRange animates a range of leds which is enabled and slowly travels all the leds
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
                
                AddLed(modRing(i), this->r * intensity, this->g * intensity, this->b * intensity, this->w * intensity);
            }
            
        
        }
        
         double LoopTimeSeconds(){
             return this->cycleTime;
         }

};

class FlickerAnimation: public Animation {

    private:
        double currentIntensity, target, r, g, b, w;

    public: 
    
    FlickerAnimation(){
        this->currentIntensity = 1.0;
        this->target = 1.0;
        this->r = 128;
        this->g = 128;
        this->b = 128;
        this->w = 128;
    }
    
    void Configure(int maxDimR, int maxDimG, int maxDimB, int maxDimW){
        this->r = maxDimR;
        this->g = maxDimG;
        this->b = maxDimB; 
        this->w = maxDimW;
    }
    
    void Animate(double time){
        
        if(randto(1.0 - this->target) < 0.05){
            // The higher the dim factor, the faster the switch
            this->target = randto(1.0);
            this->target *= this->target;// Square the number, way lower probabilities
            Serial.println("New target: "+String(this->target));
        }
        
        double diff = this->currentIntensity;
        
        DimRange(0, NUM_LEDS, (int) (1.0 * this->r * diff), (int) (1.0 * this->g * diff), (int) (1.0 * this->b * diff), (int) (this->w * diff) );
        this -> currentIntensity = (this->currentIntensity + this->target) / 2; 
    }
    
    
    double LoopTimeSeconds(){
       return 30;
    }

};

class FireAnimation: public Animation {
    private:
        int circleLength = 7;
        int startLed = 0;
        int height = NUM_LEDS / 7;
        double r, g, b, w;
        double previousTime = 0.0;
        double *intensity; 
        double *gradualIntensity; 
        int renewEvery = 20;
        
        void increaseIntensity(int i, double diff){
            if(i >= NUM_LEDS || i < 0){
                return;
            }
            double d = this->intensity[i];
            this->intensity[i] = min(1.0, max(0.0, d  + diff));
        }
        
    public:
    
        FireAnimation() {
            this->intensity = (double*) malloc(sizeof(double) * NUM_LEDS);
            this->gradualIntensity = (double*) malloc(sizeof(double) * NUM_LEDS);
        }
                
        ~FireAnimation(){
            free(this->intensity);
            free(this->gradualIntensity);
        }
    
        void Configure(int circleLength, int startLed, int height, int r, int g, int b, int w, int renewEvery){
            this->circleLength = circleLength;
            this->height = height * circleLength;
            this->startLed = startLed;
            this->r = r;
            this->g = g;
            this->b = b;
            this->w = w;
            this->renewEvery = renewEvery;
            for(int i = 0; i < NUM_LEDS; i++){
                this->intensity[i] = 0;
                this->gradualIntensity[i] = 0;
            }
        }
        
        void Animate(double time) {
        
            for(int i = this->height - 1; i >= this->startLed; i--){
                double intensity = this->gradualIntensity[i] = (5 * this->gradualIntensity[i] + this->intensity[i]) / 6;
                AddLed(modRing(i), this->r * intensity, this->g * intensity, this->b * intensity, this->w * intensity);
            }
        
            double diff = abs(time - this->previousTime);
          
            this->previousTime = time;
            if( (randto(1.0) <= 10 * diff / renewEvery)) {
                int i = (int) (randto(this->circleLength));
                double r = randto(1.25);
                this->increaseIntensity(i, r );
                this->increaseIntensity(i - 1, r * 0.2);
                this->increaseIntensity(i + 1, r * 0.2);
            }

            for(int i = this->height - 1; i >= this->startLed; i--){
                // Transfer 'diff' part of the intensity to a node above
                double intensity = this->intensity[i];
                this->intensity[i] = max(0.0, intensity * (1 - diff) - diff);
                if(randto(1.0) < diff * 5) {
                    // this->intensity[i] = max(0.0,intensity * 0.5 - speed);
                    int d = 0;
                    if(randto(1.0) < 0.1){   
                        d = 1;
                    }else if(randto(1.0) < 0.1){
                        d = -1;
                    }
                    
                    this->increaseIntensity(i + this->circleLength + d, intensity - ( this->circleLength / this->height));
                    this->intensity[i] = 0.0;
                }else{
                  // this->increaseIntensity(i + this->circleLength, intensity * 0.6 * diff);
                   //this->increaseIntensity(i + this->circleLength - 1, intensity * (0.1) * diff);
                   //this->increaseIntensity(i + this->circleLength + 1, intensity * (0.1) * diff);
                }
                                                                                
              }
        }
        
        double LoopTimeSeconds(){
            return this->height * 30;
        }

};

// This animations first dims all leds, then allows the middle leds to turn on until all the leds are enabled
// THere is a window of 25% of the leds where there is a partial gradient
class BootAnimation: public Animation {
    private:
        bool enabled = true;
        double cycleTime = 30; // in seconds
    public:
        void Disable(){
            this->enabled = false;
        }
    
        void Configure (double cycleTime) {
            this->cycleTime = cycleTime;
        }
    
        void Animate(double secondsSinceStart) {
           if(!this->enabled){
                return;
           }
           
           secondsSinceStart = (secondsSinceStart) * NUM_LEDS / this->cycleTime;
           
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
            return this->cycleTime;
        }
};