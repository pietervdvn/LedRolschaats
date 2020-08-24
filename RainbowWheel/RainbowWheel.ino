#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include <math.h> 


#define LED_PIN     D1
#define NUM_LEDS    105
#define BRIGHTNESS  100
#define LED_TYPE    WS2813
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

#define RGB_ROTATION 0
#define CLOCK 1
#define MOUNTAINS 2
#define DEBUG 3
unsigned int mode = DEBUG;

unsigned int r = 100;
unsigned int g = 100;
unsigned int b = 100;
// Width of a mountain by mode
unsigned int w = 10;
// Speed of a mountain by mode (in seconds/rotation)
unsigned int s = 5;

// PUt into mode 0 at this time
int wakeAt = 8 * 60 * 60 + 30 *60;

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


void loop(){
   handleClient(server.available());   // Listen for incoming clients
   animate();
   MDNS.update();
   
   if(totalSecondsSinceMidnight() >= wakeAt && totalSecondsSinceMidnight() <= wakeAt + 60){
        int wakeSeconds = pos(totalSecondsSinceMidnight() - wakeAt);
        mode = RGB_ROTATION;
        r = wakeSeconds;
        g = wakeSeconds;
        b = wakeSeconds;
        w = wakeSeconds;
        if(w > 60){
            w = 40;
        }
        s = 10;
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
    // MDNS.addService("http", "tcp", 80);
     Serial.println("Local domain registered!");
   }
   
 }

