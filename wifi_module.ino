#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi credentials
const char* ssid = "Fi-Wi"; // Replace with your WiFi SSID
const char* password = "hrishi999"; // Replace with your WiFi password

// ThingSpeak API parameters
const char* server = "api.thingspeak.com";
const unsigned long channelID = 2593821; // Replace with your ThingSpeak Channel ID
String apiKey = "5JCQJ8673T4OVGD0"; // Replace with your ThingSpeak Write API Key

WiFiClient client;
OneWire oneWire(2); // OneWire data pin is connected to GPIO 2
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("Connected to WiFi");
  sensors.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Generate random temperature between 30 to 37 degrees Celsius
    float tempC = random(3000, 3700) / 100.0;

    // Construct URL for ThingSpeak update
    String url = "http://" + String(server) + "/update?api_key=" + apiKey + "&field1=" + String(tempC) + "&channel_id=" + String(channelID);

    // Begin HTTP request
    HTTPClient http;
    http.begin(client, url); 

    // Send HTTP GET request
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("HTTP Response code: " + String(httpCode));
      Serial.println("HTTP Response: " + payload);
    } else {
      Serial.println("Error in sending HTTP request");
      Serial.println("HTTP Response code: " + String(httpCode));
    }

    // End HTTP request
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(10000); // Adjust delay based on your update interval
}
