/*
 * As per http://www.who.int/mediacentre/factsheets/fs313/en/
 * and per http://ec.europa.eu/environment/air/quality/standards.htm
 * WHO allows a daily mean of 25µg, and a yearly mean of 10µg (PM2.5)
 * EU allows a yearly mean of 25µg (PM2.5)
 * 
 * IN practice: seldom over 50
 * 
 * WHO allows a daily mean of 50µg and a yearly mean of 20µg (PM10)
 * EU allows a daily mean of 50µg, and a yearly mean of 40µg (PM10)
 * 
 * The color coding: over the yearly WHO norm -> yellow; over the yearly EU-norm -> red
 */
 
 #define host "data.sensor.community"

 #define httpsPort 443


unsigned int luftdatenid = 7245;

bool updatePending(){
    return totalSecondsSinceMidnight() >= luftdatenLastUpdate + 60;
}

int mapState(float value, float* barema){
  for(int state = 1; state <= EXTREMELY_BAD; state++){
    if(barema[state] > value){
       return state - 1;
    }
  }
  return EXTREMELY_BAD;
}


void updateFineDustMeasurements(){
    if(totalSecondsSinceMidnight() < luftdatenLastUpdate + 60){
        return;
    }

    if(WiFi.status() != WL_CONNECTED) {
    Serial.println("[Luftdaten] wifi not connected...");
        return;
    }
    // Use WiFiClientSecure class to create TLS connection
    WiFiClientSecure client;
    client.setInsecure();
    Serial.print("[Luftdaten] attempting to connect to ");
    Serial.println(host); 
    if (!client.connect(host, httpsPort)) {
        Serial.println("[Luftdaten] connection failed");
        luftdatenState = LUFTDATEN_DOWN;
        // Luftdaten is once again down...
        luftdatenLastUpdate = totalSecondsSinceMidnight();
        return;
    }
    Serial.println("[Luftdaten] connection succeeded");
    client.print(String("GET ") + "/airrohr/v1/sensor/" + luftdatenid+ "/ HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" +
        "User-Agent: Luchtlichtje by pietervdvn_at_posteo.net\r\n" +
        "Connection: close\r\n\r\n");
    Serial.println("[Luftdaten] request sent");
    while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
        Serial.println("headers received");
        break;
    }
  }
  client.readStringUntil('\n'); // Read the length and discard it
  String payload = client.readString();
  
  Serial.println("Got the payload");
  DynamicJsonBuffer jsonBuffer;
  JsonArray& root = jsonBuffer.parseArray(payload);
  if (!root[0].success()) {
    Serial.println("parseObject() failed");
    Serial.println(payload);
    luftdatenState = PARSE_ERROR;
    return;
  }

  float newPm10 = (root[0]["sensordatavalues"][0]["value"].as<float>() + root[1]["sensordatavalues"][0]["value"].as<float>()) / 2;
  float newPm25 = (root[0]["sensordatavalues"][1]["value"].as<float>() + root[1]["sensordatavalues"][1]["value"].as<float>()) / 2;
  if(pm10 == 0 && pm25 == 0){
        pm10 = newPm10;
        pm25 = newPm25; 
        Serial.printf("(Initial assignment) Read: %f, new value is %f\n", newPm10, pm10);
  }else{
     Serial.printf("Read: %f, old value is %f, ", newPm10, pm10);
    // The new value is calculated by taking into account the previous value as well, to have less effect of outliers
    // The shown sum equals n0/2 + n1/4 + n2/8 + n3/16 ... (the lowest index is the last measurement)
    pm10 = (newPm10 + pm10)/2.0; 
    pm25 = (newPm25 + pm25)/2.0;
    Serial.printf("New value is %f\n", pm10);
  }
  luftdatenLastUpdate = totalSecondsSinceMidnight();
  luftdatenState = UP_TO_DATE;
}
