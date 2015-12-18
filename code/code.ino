#include <Wire.h>
#include <SD.h>
#include <LiquidCrystal.h>
#include "RTClib.h"

 
RTC_DS1307 RTC;
Sd2Card card;
SdVolume volume;
SdFile root;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
const int chipSelect = 10;
File logFile;

// Pressure Sensor
const int MPX4250AP_pin = 3;
double MPX4250AP_read_value = 0;
float pressurekPa = 0;
// Temperature Sensor
const int temperatureSensorPin = 2;
// Light Sensor
const int lightSensorPin = 1;
int lightLevel;

 
void setup () {
  pinMode(10, OUTPUT);
  Serial.begin(57600);
  Wire.begin();
  RTC.begin();
  
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // uncomment it & upload to set the time, date and start run the RTC!
    //RTC.adjust(DateTime(__DATE__, __TIME__));
  }
 // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  lcd.begin(16, 2);
  // print the type of card
  Serial.print("\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      lcd.print("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      lcd.print("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      lcd.print("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }
 
}
 
void loop () {
    DateTime now = RTC.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.print(" since 1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    
    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    
    Serial.println();
    
    showAndWriteMeasurements();
    delay(10000);
}

void showAndWriteMeasurements() {
    MPX4250AP_read_value = analogRead(MPX4250AP_pin);
    pressurekPa = (MPX4250AP_read_value*(.00488)/(.022)+20);
    lcd.setCursor(0, 1); lcd.print("P:");
    lcd.setCursor(2, 1); lcd.print(pressurekPa);

    lcd.setCursor(0,0); lcd.print("C:");
    float celsius = (analogRead(temperatureSensorPin)/1024.0)*500;
    lcd.setCursor(2,0); lcd.print(celsius);

    lcd.setCursor(10,1); lcd.print("L:");
    lightLevel = analogRead(lightSensorPin);
    lcd.setCursor(12,1); lcd.print(lightLevel);
    
    lcd.setCursor(8,0);
    lcd.print(millis());
    
    logFile = SD.open("log.txt", FILE_WRITE);
    if (logFile) {
        DateTime now = RTC.now();
        logFile.print(now.year(), DEC);
        logFile.print('/');
        logFile.print(now.month(), DEC);
        logFile.print('/');
        logFile.print(now.day(), DEC);
        logFile.print(' ');
        logFile.print(now.hour(), DEC);
        logFile.print(':');
        logFile.print(now.minute(), DEC);
        logFile.print(':');
        logFile.print(now.second(), DEC);
        logFile.println();
        
        logFile.print("Pressure: ");
        logFile.println(pressurekPa);
        logFile.print("Celsius: ");
        logFile.println(celsius);
        logFile.print("Light level:");
        logFile.println(lightLevel);
        
        logFile.close();
        Serial.println("Measurements written to log file");
    } else {
      Serial.println("error opening log file");
    }
  
}
