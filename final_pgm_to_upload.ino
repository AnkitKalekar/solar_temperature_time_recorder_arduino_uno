#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "RTClib.h"
#include <SD.h>

const int chipSelect = 10; 

#define ONE_WIRE_BUS 9    
RTC_DS1307 rtc;             
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int rs = 7, en = 6, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

File file;

void setup() {
  Serial.begin(9600);
   rtc.begin();
 sensors.requestTemperatures();
   lcd.begin(16, 2);

  if (! rtc.isrunning()) 
  { 
  Serial.println("RTC is NOT running!");
  lcd.print("RTC is NOT running!");
  delay(1000);
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
  }
    else 
    {
    Serial.println("RTC is running ");
   
    lcd.print("RTC is running ");
    delay(1000);
    lcd.clear();
    }
 while (!Serial)
 {
    ; 
  }
   Serial.println("Initializing SD card..."); 
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("Card failed, or not present");
   lcd.println("  Card failed   ");
   lcd.setCursor(0, 1);
   lcd.println(" or not present ");
     delay(10000);
    return;
  }
  else
  {
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) 
  {
    dataFile.print(" Date");
    dataFile.print("      "); 
    dataFile.print(" Time");
    dataFile.print("   ");  
    dataFile.print("temp 1");
    dataFile.print(", ");
    dataFile.print("temp 2");
    dataFile.print(", ");
    dataFile.print("temp 3");
    dataFile.println();
    dataFile.close();
    }
  }
  Serial.println("card initialized.");
  lcd.println("card initialized.");
  delay(1000);
  lcd.clear();

}

void loop() {
   Serial.begin(9600);
  
   sensors.requestTemperatures(); 
   
   DateTime now = rtc.now();
    Serial.println(); 
    Serial.print(now.day());
    Serial.print('/');
    Serial.print(now.month());
    Serial.print('/');
    Serial.print(now.year());
    Serial.print(' ');
    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.print(now.second());
    Serial.print(" "); 
     if(now.second()<10)
    {
     Serial.print(" "); 
    }  
  
  for(int i =0;i<=2;i++)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(now.day());
    lcd.print('/');
    lcd.print(now.month());
    lcd.print('/');
    lcd.print(now.year());
    lcd.print(' ');
    lcd.print(now.hour());
    lcd.print(':');
    lcd.print(now.minute());
   
      lcd.setCursor(1, 1);
      lcd.print(i+1);
      lcd.print(": ");
      lcd.print(sensors.getTempCByIndex(i));   
      lcd.setCursor(10, 1);
      lcd.print("deg. C");
      
   Serial.print(" "); 
   Serial.print(i+1);
   Serial.print(":  ");
   Serial.print(sensors.getTempCByIndex(i)); 
   Serial.print(" "); 
   String dataString = "";
     delay(1000);
  }
  File dataFile = SD.open("datalog.txt", FILE_WRITE);


    float sensor1 = sensors.getTempCByIndex(0);
    float sensor2 = sensors.getTempCByIndex(1);
    float sensor3 = sensors.getTempCByIndex(2);

  if (dataFile) 
  {
    dataFile.print(now.day()); 
    dataFile.print("/");
    dataFile.print(now.month());
    dataFile.print("/");
    dataFile.print(now.year());
    dataFile.print(",");
    dataFile.print(now.hour()); 
    dataFile.print(":");
    dataFile.print(now.minute()); 
    dataFile.print(":");
    dataFile.print(now.second());
    dataFile.print(", ");
     if(now.second()<10)
    {
   dataFile.print(" "); 
    }  
    dataFile.print(sensor1);
    dataFile.print(",  ");
    dataFile.print(sensor2);
    dataFile.print(",  ");
    dataFile.print(sensor3);
    dataFile.println();
    dataFile.close();
    }
 
   else 
   {
    Serial.println("error opening datalog.txt");
    }
}
