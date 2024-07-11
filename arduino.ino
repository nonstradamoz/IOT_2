#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

// Initialize the LCD with the I2C address, columns, and rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define ONE_WIRE_BUS 9
#define TEMPERATURE_PRECISION 12

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
int numberOfDevices;
float temp;

const float DOG_FEVER_THRESHOLD = 39.2; // Adjusted for dogs
const float DOG_HYPOTHERMIA_THRESHOLD = 37.0; // Adjusted for dogs

// Pins for SoftwareSerial
#define RX 10
#define TX 11
SoftwareSerial esp8266(RX, TX);

void setup() {
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  esp8266.begin(9600);

  // Initialize temperature sensors
  sensors.begin();
  numberOfDevices = sensors.getDeviceCount();

  // Display startup message on LCD
  lcd.setCursor(0, 0);
  lcd.print("Canine Body Temp");
  lcd.setCursor(0, 1);
  lcd.print("  Monitoring");
  delay(1500);
  lcd.clear();
}

void loop() {
  sensors.requestTemperatures();
  for (int i = 0; i < numberOfDevices; i++) {
    if (sensors.getAddress(tempDeviceAddress, i)) {
      float tempC = sensors.getTempC(tempDeviceAddress);
      
      // Debugging
      Serial.println("Body Temperature: ");
      Serial.println(tempC);
      
      // Display temperature on LCD
      lcd.setCursor(0, 0);
      lcd.print("BODY TEMP:");
      lcd.setCursor(0, 1);
      lcd.print(tempC);
      lcd.print(" *C");

      // Check for fever or hypothermia
      if (tempC >= DOG_FEVER_THRESHOLD) {
        lcd.setCursor(0, 0);
        lcd.print("ALERT: FEVER     ");
        Serial.println("ALERT: FEVER");
      } else if (tempC <= DOG_HYPOTHERMIA_THRESHOLD) {
        lcd.setCursor(0, 0);
        lcd.print("ALERT: LOW TEMP  ");
        Serial.println("ALERT: LOW TEMP");
      } else {
        lcd.setCursor(0, 0);
        lcd.print("NORMAL TEMP      ");
        Serial.println("NORMAL TEMP");
      }

      // Send data to ESP8266
      String data = "temp=" + String(tempC);
      esp8266.println(data);
    }
  }
  delay(2000); // Delay for readability
}
