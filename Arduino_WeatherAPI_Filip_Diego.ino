/*
Autors: Filip Slavkovic and Diego Fantino
Version: 1.10 
Short description: For this code you must have at least a M5StickC to run this code and a Hotspot for example a Mobile Hotspot. 
The code works with a setup and a loop method. In the method setup, I set up a wifi connection and connected with the OpenWeatherMapApi. 
After that I displayed some information on the LCD-Display of the M5StickC. In the loop method, I did quiet the same, but the data gets refereshed every 10 seconds.
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <M5StickC.h>

const char* ssid = "filipmornshuel";
const char* password = "MornShuel123";

//Api key
String openWeatherMapApiKey = "2ddf6fc882acd30e2110bdfa69facd14";


//Constant for the api key
String city = "Zurich";
String countryCode = "CH";
String language = "de";

// server path with UML + the constants
String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode +"&APPID=" + openWeatherMapApiKey +"&units=metric";

unsigned long lastTime = 0;

//Timer 10 seconds
unsigned long timerDelay = 10000;

String jsonBuffer;
 
//Setup method
void setup() {
  M5.begin();
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
  
  //Checking if the wifi is connected with the m5stickc
  if(WiFi.status()== WL_CONNECTED){
     
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);
  
      
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      //displaying the informations on the lcd-display of the m5stickc
      M5.Lcd.println("Zurich ");
      M5.Lcd.println("Temp °C: "); 
      M5.Lcd.println(myObject["main"]["temp"]);
      M5.Lcd.println("Description: ");
      M5.Lcd.println(myObject["weather"][0]["description"]);
      M5.Lcd.println("Pressure: ");
      M5.Lcd.println(myObject["main"]["pressure"]);
      M5.Lcd.println("Humidity: ");
      M5.Lcd.println(myObject["main"]["humidity"]);
      M5.Lcd.println("Wind Speed: ");
      M5.Lcd.println(myObject["wind"]["speed"]);
     
      
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    
}

//Loop Method
void loop() {
  
  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay) {
    
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      
      //String test = "http://api.openweathermap.org/data/2.5/weather?q=" + 

      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);
    
      
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      // Console print for test
      /*
      Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.print("Temperature: ");
      Serial.println(myObject["main"]["temp"]);
      Serial.print("Pressure: ");
      Serial.println(myObject["main"]["pressure"]);
      Serial.print("Humidity: ");
      Serial.println(myObject["main"]["humidity"]);
      Serial.print("Wind Speed: ");
      Serial.println(myObject["wind"]["speed"]);
      */
      
      //Displaying the data
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.println("Zurich ");
      M5.Lcd.println("Temp °C: "); 
      M5.Lcd.println(myObject["main"]["temp"]);
      M5.Lcd.println("Description: ");
      M5.Lcd.println(myObject["weather"][0]["description"]);
      M5.Lcd.println("Pressure: ");
      M5.Lcd.println(myObject["main"]["pressure"]);
      M5.Lcd.println("Humidity: ");
      M5.Lcd.println(myObject["main"]["humidity"]);
      M5.Lcd.println("Wind Speed: ");
      M5.Lcd.println(myObject["wind"]["speed"]);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  
}

//String for the HTTP get request
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    

  http.begin(client, serverName);
  
  
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();

  return payload;
}