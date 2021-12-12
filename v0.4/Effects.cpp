#include <Arduino.h>
#include "Lights.h"
#include "Properties.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int modRing(int i){
    while(i < 0){
        i += NUM_LEDS;
    }
    while(i >= NUM_LEDS){
        i -= NUM_LEDS;
    }
    return i;
}

/* generate a random floating point number from min to max */
double randto(double range) 
{
    double div = RAND_MAX / range;
    return ((random() / div));
}
    
 void DimRangeGradual(int from, int to, int rDim, int gDim, int bDim, int wDim){
    if(from > to){
     for(int i = to; i < from; i++){
            double startWeight = ((double) from - i)/(from - to);
            DimLed(modRing(i), startWeight * rDim, startWeight * gDim,startWeight * bDim, startWeight * wDim  );
        }
    }else{
        for(int i = from; i < to; i++){
            double startWeight = 1.0-((double) to - i)/(to - from);
            DimLed(modRing(i), startWeight * rDim, startWeight * gDim,startWeight * bDim, startWeight * wDim  );
        
        }
    }

}
        
 void AddRangeGradual(int from, int to, int rDim, int gDim, int bDim, int wDim){
    if(from > to){
     for(int i = to; i < from; i++){
            double startWeight = 1.0 - ((double) from - i)/(from - to);
            AddLed(modRing(i), startWeight * rDim, startWeight * gDim,startWeight * bDim, startWeight * wDim  );
        }
    }else{
        for(int i = from; i < to; i++){
            double startWeight = ((double) to - i)/(to - from);
            AddLed(modRing(i), startWeight * rDim, startWeight * gDim,startWeight * bDim, startWeight * wDim  );
        
        }
    }

}
        
void SetRange(int from, int to, int r, int g, int b, int w){
    for(int i = from; i < to; i++){
        SetLed(modRing(i), r, g, b, w  );
    
    }
}
         
void DimRange(int from, int to, int r, int g, int b, int w){
  for(int i = from; i < to; i++){
      DimLed(modRing(i), r, g, b, w  );
  
  }
}
         
void AddRange(int from, int to, int r, int g, int b, int w){
 for(int i = from; i < to; i++){
    AddLed(modRing(i), r, g, b, w  );
 }
}

void CountLeds(){
    for(int i = 0; i < NUM_LEDS; i++){
        SetLed(modRing(i),
         (i % 5 == 0) ? 255 : 0,  // Red every 5
        (i % 2 == 0) ? 255 : 0, // Green every 2
        (i % 25 == 0) ? 255 : 0, // BLue every 25
        (i % 10 == 0) ? 255 : 0// WHite every 10
        );
    }
}
