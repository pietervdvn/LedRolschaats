#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
// #include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <math.h> 
#include <ArduinoJson.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>


#define LED_PIN     D1
#define NUM_LEDS    106
#define BRIGHTNESS  100
#define LED_TYPE    WS2813
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

#define RGB_ROTATION 0
#define CLOCK 1
#define MOUNTAINS 2
#define SECTORS 3
#define LUFTDATEN 4

unsigned int mode = 0;

unsigned int r = 100;
unsigned int g = 100;
unsigned int b = 100;
// Width of a mountain by mode
unsigned int w = 10;
// Speed of a mountain by mode (in seconds/rotation)
unsigned int s = 5;

// PUt into mode 0 at this time (seconds past midnight)
int wakeAt = 8 * 60 * 60 + 30 *60;




#define EXTREMELY_GOOD 0
#define VERY_GOOD 1
#define GOOD 2
#define LOW_AVG 3
#define AVG 4
#define HIGH_AVG 5
#define BAD 6
#define VERY_BAD 7
#define EXTREMELY_BAD 8


float* barema25 = new float[9] {0, /*GOOD:*/5,  7.5, 10, /*AVG*/ 15,   20,   25,/*BAD*/ 35, 45};
float* barema10 = new float[9] {0, /*GOOD:*/7.5, 15, 20, /*AVG*/ 26.6, 33.3, 40,/*BAD*/ 50, 80};
float pm10 = 0;
float pm25 = 0;
int luftdatenLastUpdate = 0;



#define NOT_YET_ASKED 0
#define UP_TO_DATE 1
#define LUFTDATEN_DOWN 2
#define PARSE_ERROR 3
int luftdatenState = NOT_YET_ASKED;


String Header;
// Set web server port number to 80
WiFiServer server(80);

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

unsigned long timeOfBootMillis = 0;

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  
  clear();
  setLed(0,0,50,0);
  FastLED.show();
        
  setupWifi();
  
}

// Connect to Wi-Fi network with SSID and password
void setupWifi(){
  WiFi.hostname("rainbow");
  wifi_station_set_hostname("rainbow");
  WiFi.begin("woonkamer","elfjuli9");
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    animate();
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  AdvertiseServices();
  
  server.begin();
}

double analogAvg = 0;

#define measurementCount 25
int measurements [measurementCount];
int currentIndex = -1;
double detectTouch(){
    if(currentIndex == -1){
        for(int i = 0; i < measurementCount; i++){
            measurements[i] = 0;
        }
        currentIndex = 0;
    }
    measurements[currentIndex] = analogRead(A0);
    Serial.print("> ");
    Serial.println(measurements[currentIndex]);
    currentIndex = (currentIndex + 1) % measurementCount;
    int sum = 0;
    for(int i = 0; i < measurementCount; i++){
        sum += measurements[i];
    }
    return 1.0 * sum / measurementCount;
}

void loop(){

   animate();
   handleClient(server.available());   // Listen for incoming clients
   MDNS.update();
   if(currentMinutes() == 0){
      MDNS.update();
   }
   
   if(totalSecondsSinceMidnight() >= wakeAt && totalSecondsSinceMidnight() <= wakeAt + 60){
        int wakeSeconds = pos(totalSecondsSinceMidnight() - wakeAt);
        mode = RGB_ROTATION;
        r = wakeSeconds;
        g = wakeSeconds;
        b = wakeSeconds;
        w = wakeSeconds;
        if(w <= 0){
            w = 1;
        }
        if(w > 80){
            w = 80;
        }
        s = 5;
   }
   
}


int currentSeconds(){
    return totalSecondsSinceMidnight() % 60;
}

int currentMinutes(){
    return (totalSecondsSinceMidnight() / 60) % 60;
}

int currentHours(){
    return (totalSecondsSinceMidnight() / (60 * 60)) % 24;
}


inline long totalMillisSinceMidnight(){
    return timeOfBootMillis + (system_get_time() / 1000);
}

inline long totalSecondsSinceMidnight(){
    return totalMillisSinceMidnight() / 1000;
}

void AdvertiseServices() {
   
   if (MDNS.begin("rainbow"))
   {
     // Add service to MDNS-SD
     MDNS.addService("http", "tcp", 80);
     Serial.println("Local domain registered!");
   }
   
 }
/*
///////////////////////////
// capacitive sense for door handle
///////////////////////////
uint8_t readCapacitivePin(int pinToMeasure) {
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Prevent the timer IRQ from disturbing our measurement
  noInterrupts();
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = MAX_CAP;
       if (*pin & bitmask) { cycles =  0;}
  else if (*pin & bitmask) { cycles =  1;}
  else if (*pin & bitmask) { cycles =  2;}
  else if (*pin & bitmask) { cycles =  3;}
  else if (*pin & bitmask) { cycles =  4;}
  else if (*pin & bitmask) { cycles =  5;}
  else if (*pin & bitmask) { cycles =  6;}
  else if (*pin & bitmask) { cycles =  7;}
  else if (*pin & bitmask) { cycles =  8;}
  else if (*pin & bitmask) { cycles =  9;}
  else if (*pin & bitmask) { cycles = 10;}
  else if (*pin & bitmask) { cycles = 11;}
  else if (*pin & bitmask) { cycles = 12;}
  else if (*pin & bitmask) { cycles = 13;}
  else if (*pin & bitmask) { cycles = 14;}
  else if (*pin & bitmask) { cycles = 15;}
  else if (*pin & bitmask) { cycles = 16;}

  // End of timing-critical section
  interrupts();

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  return cycles;
}*/