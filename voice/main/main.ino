
  
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
#include <WiFi.h>
#include <WiFiClient.h>

#include <ArduinoJson.h>

/**        
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.

uint8_t records[7]; // save record
uint8_t buf[64];

int led = 13;

#define on    (0)
#define off   (1) 
#define search  (2)

char ssid[] = "Akhinani.2g";
char password = "akhinani15%";

// Wikipedia API settings
const char* wikipediaEndpoint = "en.wikipedia.org";
const int wikipediaPort = 80;
const char* wikipediaPage = "/w/api.php?action=query&format=json&prop=extracts&exintro=&explaintext=&titles=";

// Wi-Fi module pins
const int wifiRxPin = 4;  // RX pin of the Wi-Fi module connected to Arduino TX pin
const int wifiTxPin = 5;  // TX pin of the Wi-Fi module connected to Arduino RX pin

SoftwareSerial wifiSerial(wifiRxPin, wifiTxPin);
WiFiClient client(wifiSerial);

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized. 
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
  /** initialize */
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  
  pinMode(led, OUTPUT);
    
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)on) >= 0){
    Serial.println("onRecord loaded");
  }
  
  if(myVR.load((uint8_t)off) >= 0){
    Serial.println("offRecord loaded");
  }

   WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case on:
        /** turn on LED */
        digitalWrite(led, HIGH);
        break;
      case off:
        /** turn off LED*/
        digitalWrite(led, LOW);
        break;
      case search:
      String searchTerm = "Arduino";
      Serial.print("Search Term: ");
      Serial.println(searchTerm);
      retrieveWikipediaInformation(searchTerm);
    }
    /** voice recognized */
    printVR(buf);
  }
}

void retrieveWikipediaInformation(String searchTerm) {
  // Connect to Wikipedia API
  if (client.connect(wikipediaEndpoint, wikipediaPort)) {
    Serial.println("Connected to Wikipedia API");

    // Send GET request
    String requestUrl = String(wikipediaPage) + searchTerm;
    client.print(String("GET ") + requestUrl + " HTTP/1.1\r\n" +
                 "Host: " + wikipediaEndpoint + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(1000);

    // Read response
    String response = "";
    while (client.available()) {
      String line = client.readStringUntil('\r');
      response += line;
    }

    // Parse and extract Wikipedia information
    const size_t capacity = JSON_OBJECT_SIZE(3) + 200;
    DynamicJsonDocument doc(capacity);

    DeserializationError error = deserializeJson(doc, response);
    if (error) {
      Serial.print("Error parsing JSON: ");
      Serial.println(error.c_str());
    } else {
      JsonObject query = doc["query"];
      JsonObject pages = query["pages"];

      // Get the first page
      const char* pageId = pages.begin()->key().c_str();
      JsonObject page = pages[pageId];

      const char* title = page["title"];
      const char* extract = page["extract"];

      // Print Wikipedia information to the serial monitor
      Serial.println("\nWikipedia Information:");
      Serial.print("Title: ");
      Serial.println(title);
      Serial.print("Extract: ");
      Serial.println(extract);
    }

    client.stop();
    Serial.println("Disconnected from Wikipedia API");
  }
}
