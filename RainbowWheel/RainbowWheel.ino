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

String wifi_ssid = "";
String wifi_password = "";
String hostname = "rainbow-wheel";

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

void setupAccessPoint(){

    clear();
    setLed(0, 100,0,0);
    setLed(NUM_LEDS / 2, 100,0,0);
    FastLED.show();

}

// Connect to Wi-Fi network with SSID and password
void setupWifi(){
  WiFi.hostname(hostname);
  wifi_station_set_hostname(hostname);
  
  
  if(wifi_ssid == ""){
    setupAccessPoint();
    return;
  }
  
  WiFi.begin(wifi_ssid,wifi_password);
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    
    clear();
    setLed(count, 100,100,0);
    FastLED.show();
    
    Serial.print(".");
    count ++;
    if(count > 200*30){
        // We reached a timeout
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


int lastMdnsUpdate = -15*60;

void loop(){

   animate();
   handleClient(server.available());   // Listen for incoming clients
   MDNS.update();
   if(totalSecondSinceMidnight() >= lastMdnsUpdate + 15*60){
      MDNS.update();
      lastMdnsUpdate = totalSecondSinceMidnight();
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
 