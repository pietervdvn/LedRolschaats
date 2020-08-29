

void animate(){
    clear();
    switch(mode){
        case RGB_ROTATION: animateRGBBow(); break;
        case CLOCK: animateClock(); break;
        case MOUNTAINS: animateDragons(); break;
        case SECTORS: debugLights(); break;
        case LUFTDATEN: showAndUpdateLuftdaten(); break;
    }
    
    FastLED.show();
    FastLED.delay(10);
}

void debugLights(){
    for(int i = 0; i < 12; i++){
        if( i == currentSeconds() % 12){
            continue;
        }
        setSector(i, (i % 2) * r, ((i+1)%2) * g, 0 );
    }
   
    setSector(currentSeconds(), 0,0,b);
}


void setSector(int sectorN, int r, int g, int b){
    sectorN = sectorN % 12;
    int lengths[] = {/*12 o'clock*/9,9,8, /*3 oclock*/9,9,9, /*6 o'clock*/ 9,8,9,  9,9,9};
    int start = 0;
    int stop = 0;
    for(int i = 0; i <= sectorN; i++){
        start = stop;
        stop = start + lengths[i];
    }
   
    start -= 4;
    stop -= 4;
    
    for(int i = start; i < stop; i++){
        addLed(i, r, g, b);
    }
    
}

void animateClock(){
    int s = (currentSeconds() * NUM_LEDS) / 60;
    int m = (currentMinutes() * NUM_LEDS) / 60;
    
    
    setSector(currentHours(), 0,0, b);
    setLed(m-2, 0,g,0);
    setLed(m-1, 0,g,0);
    setLed(m, 0,g,0);
    setLed(s, r,0,0);
       

}

void animateRGBBow(){
     double millisSinceStart = totalMillisSinceMidnight();
     double redCenter = loopLocation(millisSinceStart, s * 1000);
     setLedMountain(redCenter, r,0,0, w);
     double greenCenter = loopLocation(millisSinceStart, s * 1000) + NUM_LEDS / 3;
     setLedMountain(greenCenter, 0,g,0, w);
     double blueCenter = loopLocation(millisSinceStart, s * 1000) + 2 * NUM_LEDS / 3;
     setLedMountain(blueCenter, 0,0,b, w);
}

void showAndUpdateLuftdaten(){
    showLuftDaten();
    if(updatePending()){
        setLed(0,255,255,255);
        setLed(NUM_LEDS / 2,255,255,255);
        FastLED.show();
        Serial.println("Requesting data update...");
        updateFineDustMeasurements();
        setLed(0,0,0,0);
        setLed(NUM_LEDS / 2,0,0,0);
        FastLED.show();
    }
}

void showLuftDaten(){

    if(luftdatenState == NOT_YET_ASKED){
        setSector(currentSeconds(), 0,100,0);
        return;
    }
    
    if(luftdatenState == LUFTDATEN_DOWN){
        setSector(currentSeconds(), 100,100,0);
        return;
    }
    
    if(luftdatenState == PARSE_ERROR){
        setSector(currentSeconds(), 100,0,100);
        return;
    }    
    
    showEncodedLights(3 * NUM_LEDS / 4, pm25, barema25);
    showEncodedLights(NUM_LEDS / 4, pm10, barema10);
}

void showEncodedLights(int center, float pmstate, float* barema){
    int encodedState = mapState(pmstate, barema);
    int r = 0;
    int g = 0;
    int b = 0;
    double width = ((NUM_LEDS - 5) / 2) * pmstate / barema[EXTREMELY_BAD];
    switch(encodedState){
        case EXTREMELY_GOOD:
        case VERY_GOOD:
        case GOOD:
            g = 200;
            break;
        case LOW_AVG:
        case AVG:
        case HIGH_AVG:
            r = 200;
            g = 200;
            break;
        case BAD:
        case VERY_BAD:
        case EXTREMELY_BAD:
            r = 200;
            break;
    }
    setLedMountain(center, r, g, b, min(0.0+width,0.0+ NUM_LEDS / 2));
}

void animateDragons(){
    double millisSinceStart = totalMillisSinceMidnight();
    double rpeak = loopLocation(millisSinceStart, s * 1000 * 1);
    double bpeak = - loopLocation(millisSinceStart, s * 1000 * 3) + 2 * NUM_LEDS / 3;
    double gpeak = loopLocation(millisSinceStart, s * 1000 * 5) + NUM_LEDS / 3;
    double ypeak = -loopLocation(millisSinceStart, s * 1000 * 7) + NUM_LEDS / 3;
    
    setLedMountain(rpeak, r, 0, 0, 12);
    setLedMountain(gpeak, 0, g, 0, 12);
    setLedMountain(bpeak, 0, 0, b, 9);
    setLedMountain(ypeak, r, g, 0, 6);
    
}


// Creates a hue, with 'center' as brightest point
void setLedMountain(double center, int rbright, int gbright, int bbright, int ttl){
    if(ttl <= 0){
        return;
    }
    center = modring(center);
    for (int i = 0; i < NUM_LEDS; i++) {
        double diff = distance(i, center);
        double rDiff = 0.0  + (rbright * diff)/ttl;
        double gDiff = 0.0  + (gbright * diff)/ttl;
        double bDiff = 0.0  + (bbright * diff)/ttl;
        addLed(i, rbright - rDiff, gbright - gDiff, bbright - bDiff);
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

double loopLocation(double currentTime, double rotationTime){
   if(rotationTime < 0){  
        return -(NUM_LEDS * mod(currentTime, rotationTime) / -rotationTime);
   }
   return NUM_LEDS * mod(currentTime, rotationTime) / rotationTime;
}


double mod(double d, double operand){
    operand = dabs(operand);
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

inline double pos(double d){
    if(d < 0){
        return 0;
    }
    return d;
}

void clear(){
    for(int i = 0; i < NUM_LEDS; i++){
        leds[i] = CRGB(0,0,0);
    }
}

void setLed(int i, double r, double g, double b){
    i = modring(i);
	leds[i] = CRGB(pos(r),pos(g), pos(b));
}

void addLed(int i, double r, double g, double b){
    i = modring(i);
	leds[i] |= CRGB(pos(r),pos(g), pos(b));
}


