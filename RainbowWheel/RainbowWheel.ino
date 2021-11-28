#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
// #include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <math.h> 
// #include <ArduinoJson.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include <EEPROM.h>
#include "FastLED_RGBW.h"

#define LED_PIN     D1
#define NUM_LEDS    105
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER RGB

// Should be high to let the led shine
#define STATUS_LED D7

#define CAPACITIVE_READ_PIN A0
#define CAPACITIVE_SEND_PIN D6



// FastLED with RGBW
CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];

#define RGB_ROTATION 0
#define CLOCK 1
#define MOUNTAINS 2
#define SECTORS 3
#define LUFTDATEN 4
#define CHRISTMAS 5
#define OFF 6
#define ACCESS_POINT_MODE 1000

unsigned int mode = 0;

unsigned int r = 100;
unsigned int g = 100;
unsigned int b = 100;
// Width of a mountain by mode
unsigned int w = 10;
// Speed of a mountain by mode (in seconds/rotation)
unsigned int s = 5;

double touchExtra = 0;
int touchCalibration = 2900;

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
#define ACCESS_POINT_MODE 1000
int luftdatenState = NOT_YET_ASKED;


String Header;
// Set web server port number to 80
WiFiServer server(80);

String wifi_ssid = "";
String wifi_password = "";
String hostname = "rainbow";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

unsigned long timeOfBootMillis = 0;

 void writeStringToEEPROM(int addrOffset, const String &strToWrite)
 {
    byte len = strToWrite.length();
    EEPROM.write(addrOffset, len);
    for (int i = 0; i < len; i++)
    {
        EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
    }
    EEPROM.commit();
    Serial.print("Written string to eeprom: index: ");
    Serial.print(addrOffset);
    Serial.print("; length: ");
    Serial.print(len);
 }
 
  void writeIntToEEProm(int addrOffset, int value)
  {
    byte len = 4;
    for(int i = 0; i < len; i++){
        EEPROM.write(addrOffset + i, value % 256);
        value = value / 256;
    }
    EEPROM.commit();
  }
  
    int readIntFromEEProm(int addrOffset)
    {
      byte len = 4;
      int value = 0;
      for(int i = 3; i >= 0; i--){
          value = value * 256;
          value += EEPROM.read(addrOffset + i);
      }
      return value;
    }
    
  
 
 String readStringFromEEPROM(int addrOffset) {
   int newStrLen = EEPROM.read(addrOffset);
   char data[newStrLen + 1];
   for (int i = 0; i < newStrLen; i++)
   {
     data[i] = EEPROM.read(addrOffset + 1 + i);
   }
   data[newStrLen] = '\0';
   return String(data);
 }



#define WIFI_SSID_LOCATION 10
#define WIFI_PASSWORD_LOCATION 265
#define HOSTNAME_LOCATION 522
#define LUFTDATEN_LOCATION 778

void setup() {
    Serial.begin(115200);
  
    pinMode(CAPACITIVE_READ_PIN, INPUT);
    pinMode(CAPACITIVE_SEND_PIN, OUTPUT);
  
  
   FastLED.addLeds<WS2812B, LED_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
    FastLED.setBrightness(  BRIGHTNESS );
  
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, HIGH);
    
    /*
    * We save 3 strings of max 255 chars:
    * wifi_ssid
    * wifi_password
    * hostname
    *
    * Furthermore, we save a telltale-string at location 0 namely "rainbow" in order to see if things are initialized already
    * At last, we save one extra int for the luftdaten-id
    */
    EEPROM.begin(256*3 + 10 + 4);
    delay(150);
    String telltale = readStringFromEEPROM(1);
    Serial.println("Telltale is:");
    Serial.println(telltale);
    if(telltale.length() == 7 && telltale.equals("rainbow")){
      Serial.println("This device has run the rainbow-led code before");
    }else{
      Serial.println("Initializing EEPROM");
      writeStringToEEPROM(WIFI_SSID_LOCATION, "");
      writeStringToEEPROM(WIFI_PASSWORD_LOCATION, "");
      writeStringToEEPROM(HOSTNAME_LOCATION, "");
      writeStringToEEPROM(1, "rainbow");
      Serial.println("Init done");
    }
                clear();
    
    setLed(0, 100, 100, 0);
    setLed(NUM_LEDS/3, 100, 100, 0);
    setLed(2 * NUM_LEDS/3, 100, 100, 0);
    
    commitLeds();
    setupWifi();
 
}

bool accessPointStarted = false;
void setupAccessPoint(){
    if(accessPointStarted){
        return;
    }
    accessPointStarted = true;
    if(hostname.length() == 0){
        hostname= "rainbow-pietervdvn";
    }
    WiFi.softAP("rainbow-ledstrip-pietervdvn");
    mode = ACCESS_POINT_MODE;
    server.begin();
    Serial.println("AP setup done");
}


// Connect to Wi-Fi network with SSID and password
void setupWifi(){

    
    wifi_ssid = readStringFromEEPROM(WIFI_SSID_LOCATION);
    wifi_password = readStringFromEEPROM(WIFI_PASSWORD_LOCATION);
    hostname = readStringFromEEPROM(HOSTNAME_LOCATION);

    WiFi.hostname(hostname);
    if(wifi_ssid.length() == 0){
        Serial.println("Wifi ssid is empty -> using access point");
        setupAccessPoint();
        return;
    }
  
  Serial.print("Attempting to connect to wifi ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid,wifi_password);
  int count = 10 * 60;
  int maxCount = count;
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    
    clear();
    
    int maxLed = (1.0 * NUM_LEDS * count / maxCount);
    for(int i = 0; i < maxLed; i++){
        setLed(i, 0,100,0);
    }
    
    FastLED.show();
    count --;
    Serial.print(".");
    if(count % 100 == 0){
        Serial.print("\nAttempting to connect with ");
        Serial.print(wifi_ssid);
        Serial.print(" ");
        Serial.println(wifi_password);
    }
    if(count <= 0){
        // We reached a timeout
        Serial.println("Waiting for wifi connection: timeout. Using AP-mode");
        setupAccessPoint();
        return;
    }
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  AdvertiseServices();
  
  server.begin();
  mode = RGB_ROTATION;
  calibrateTouch();
}

#define CAPACITIVE_MEASUREMENT_COUNT 50
int cyclesSinceLastTouch = 1000;
/*
THe analog pin is directly connected with the wheel, the CAPACITVE_SEND_PIN is connected to the wheel via a 1M-Ohm resistor.
By setting the pin to high, the wheel will "slowly" rise it's voltage.
By measuring the time needed, we can detect touch events on the wheel
*/
int readTouchValue(){
    noInterrupts();
    digitalWrite(CAPACITIVE_SEND_PIN, HIGH);
    int measurements[CAPACITIVE_MEASUREMENT_COUNT];
    for(int i = 0; i < CAPACITIVE_MEASUREMENT_COUNT; i++){
        measurements[i] = analogRead(CAPACITIVE_READ_PIN);
    }
    digitalWrite(CAPACITIVE_SEND_PIN, LOW);
    interrupts();
    delay(25);
    
    int sum = 0;
    for(int i = 0; i < CAPACITIVE_MEASUREMENT_COUNT; i++){
           sum += measurements[i];
    }
    return sum;
}

#define CALIBRATION_TIME_SECONDS 30
void calibrateTouch(){
    Serial.println("Calibrating touch");
    int calibrationStart = totalMillisSinceMidnight();
    touchCalibration = 0;
    while(totalMillisSinceMidnight() < calibrationStart + CALIBRATION_TIME_SECONDS * 1000){
        int touchValue = readTouchValue();
        if(touchValue > touchCalibration){
            Serial.println("New touch value");
            Serial.println(touchValue);
            touchCalibration = touchValue;
        }
        clear();
        int timePassed = (totalMillisSinceMidnight() - calibrationStart);
        setLed(NUM_LEDS * timePassed / (CALIBRATION_TIME_SECONDS * 1000),0,0,100);
        FastLED.show();
    }
    touchCalibration = touchCalibration * 110 / 100;
}

void loop(){
    int touchValue = readTouchValue();
    cyclesSinceLastTouch ++;
    if(cyclesSinceLastTouch > 1000){
        cyclesSinceLastTouch = 1000;
    }
    if((touchCalibration > 0 &&  touchValue > touchCalibration) || (touchCalibration < 0 && touchValue < -touchCalibration)){
        cyclesSinceLastTouch = 0;
        touchExtra += 1;
    }
    
    if(cyclesSinceLastTouch < 5){
        touchExtra += 1;
        digitalWrite(STATUS_LED, HIGH);
    }else {
        digitalWrite(STATUS_LED, LOW);
        touchExtra = touchExtra - 0.1;
    }
    
    if(touchExtra > 2048){
        touchExtra = 2048;
    }
    
    if(touchExtra < 0){
        touchExtra = 0;
    }
    
    if(mode == ACCESS_POINT_MODE && totalSecondsSinceMidnight() % 120 == 0){
        setupWifi();
    }
    

    animate();
    handleClient(server.available());   // Listen for incoming clients
    MDNS.update();
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
   
   if (MDNS.begin(hostname))
   {
     // Add service to MDNS-SD
     MDNS.addService("http", "tcp", 80);
     MDNS.update(); 
     Serial.println("Local domain registered!");
   }
   
 }
 
