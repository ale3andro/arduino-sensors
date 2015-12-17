#include <Wire.h>
#include "RTClib.h"
#include <SD.h>
#include <LiquidCrystal.h>
#include <dht.h>

 
RTC_DS1307 RTC;
Sd2Card card;
SdVolume volume;
SdFile root;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
dht DHT;

const int chipSelect = 10;
// Pressure Sensor
const int MPX4250AP_pin = 3;
double MPX4250AP_read_value = 0;
float pressurekPa = 0;
// Temperature Sensor
const int temperatureSensorPin = 2;
const int lightSensorPin = 1;
int lightLevel, high = 0, low = 1023;

 
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
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
   Serial.println("Wiring is correct and a card is present."); 
  }
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
    
    showMeasurementsOnLCD();
    delay(3000);
}

void showMeasurementsOnLCD() {
    MPX4250AP_read_value = analogRead(MPX4250AP_pin);
    pressurekPa = (MPX4250AP_read_value*(.00488)/(.022)+20);
    lcd.setCursor(0, 1); lcd.print("P:");
    lcd.setCursor(2, 1); lcd.print(pressurekPa);

    lcd.setCursor(0,0); lcd.print("C:");
    float celsius = (analogRead(temperatureSensorPin)/1024.0)*500;
    lcd.setCursor(2,0); lcd.print(celsius);

    lcd.setCursor(10,1); lcd.print("L:");
    lcd.setCursor(12,1); lcd.print(analogRead(lightSensorPin));
    
    lcd.setCursor(8,0);
    lcd.print(millis());
  
}
