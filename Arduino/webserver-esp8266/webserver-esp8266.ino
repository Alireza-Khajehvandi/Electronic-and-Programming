
// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
// Replace with your network credentials
const char* ssid = "your-ssid";
const char* password = "your-password";
int cnt=0;
// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "OFF";
    }
    else{
      ledState = "ON";
    }
    Serial.print("var: ");
    Serial.print(var);
    Serial.print(" : ");
    Serial.println(ledState);
    return ledState;
  }
  Serial.print("alireza: ");
  Serial.println(String());
  return String();
}
// Replaces placeholder with LED state value
String processor1(const String& var1){
  Serial.println(var1);
  if(var1 == "ClickMeSTATE"){
    if(cnt == 1){
      ledState = "@alireza_khajehvandi";
    }
    else if (cnt == 2){
      ledState = "www.linkedin.com/in/alireza-khajehvandi";
    }
    else{
      ledState = "github.com/alireza-khajehvandi";
    }
    Serial.print("var: ");
    Serial.print(var1);
    Serial.print(" : ");
    Serial.println(ledState);
    return ledState;
  }
  Serial.print("alireza: ");
  Serial.println(String());
  return String();
}

 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to set GPIO to LOW
  server.on("/Linkedin", HTTP_GET, [](AsyncWebServerRequest *request){ 
    cnt = 2;  
    request->send(SPIFFS, "/index.html", String(), false, processor1);
  });

  // Route to set GPIO to LOW
  server.on("/Instagram", HTTP_GET, [](AsyncWebServerRequest *request){ 
    cnt = 1;  
    request->send(SPIFFS, "/index.html", String(), false, processor1);
  });
  // Route to set GPIO to LOW
  server.on("/Github", HTTP_GET, [](AsyncWebServerRequest *request){
    cnt = 0;  
    request->send(SPIFFS, "/index.html", String(), false, processor1);
  });
  // Start server
  server.begin();
}
 
void loop(){
  
}
