/*
Code based on examples provided by:
 http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
 */

/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/* RTC */
#include <stdio.h>
#include <string.h>
#include <DS1302.h>
/* LCD 1602 */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define NRF_CE_PIN   9
#define NRF_CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

/*-----( Declare objects )-----*/
RF24 radio(NRF_CE_PIN, NRF_CSN_PIN); // Create a Radio

/*-----( Declare Variables )-----*/
int data[6];

/* RTC */
uint8_t RTC_CE_PIN   = 5;
uint8_t RTC_IO_PIN   = 6;
uint8_t RTC_SCLK_PIN = 7;

/* Create buffers */
char buf[50];
char day[10];

/* Create a DS1302 object */
DS1302 rtc(RTC_CE_PIN, RTC_IO_PIN, RTC_SCLK_PIN);


LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int lightSensorCode = 1;
const int soilMoistureSensorCode = 2;
const int temperatureSensorCode = 3;

int sensorValues[3];

int sensorCodes[3];

int sensorPosition = 0;

int currentId[3];
/*char lightSensorLetter = "L";
 char soilMoistureSensorLetter = "M";
 char temperatureSensorLetter = "T";
 */

long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 250; 

void setup()
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();

  lcd.init();                      // initialize the lcd 

  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Loading...");

  sensorValues[0] = 0;
  sensorValues[1] = 0;
  sensorValues[2] = 0;

  sensorCodes[0] = 0;
  sensorCodes[1] = 0;
  sensorCodes[2] = 0;

  currentId[0] = 0;
  currentId[1] = 0;
  currentId[2] = 0;
}

void loop()
{
  if ( radio.available() )
  {
    // Read the data payload until we've received everything
    bool done = false;
    while (!done)
    {

      // Fetch the data payload
      done = radio.read( data, sizeof(data) );
      Serial.print("D;");
      Serial.print("T:");
      Serial.print(getTime());
      Serial.print(";");
      Serial.print("Id:");
      Serial.print(data[0]);
      Serial.print(".");
      Serial.print(data[1]);
      Serial.print(".");
      Serial.print(data[2]);
      Serial.print(";");
      Serial.print("#:");
      Serial.print(data[3]);
      Serial.print("S:");
      Serial.print(data[4]);
      Serial.print(";");
      Serial.print("V:");   
      Serial.print(data[5]);
      Serial.print(";");
      Serial.println();
      
      int sensorNumber = data[3];

      sensorValues[sensorNumber-1] = data[5];
      sensorCodes[sensorNumber-1] = data[4];
      Serial.println(sensorPosition);

      currentId[0] = data[0];
      currentId[1] = data[1];
      currentId[2] = data[2];

      if (sensorPosition >= 2)
        refreshLCD();

      if (sensorPosition >= 2)
        sensorPosition = 0;
      else
        sensorPosition++;

    }
    sensorPosition = 0;
  }
  else
  {    
    Serial.println("No radio available");
    sensorPosition = 0;
  }
}

char* getTime()
{
  char dateTime[60];

  /* Get the current time and date from the chip */
  Time t = rtc.time();

  /* Format the time and date */
  snprintf(dateTime, sizeof(dateTime), "%04d-%02d-%02d %02d:%02d:%02d",
  t.yr, t.mon, t.date,
  t.hr, t.min, t.sec);

  return dateTime;
}

char getSensorLetter(int sensorCode)
{
  switch(sensorCode)
  {
  case lightSensorCode:
    return 'L';//lightSensorLetter;
  case temperatureSensorCode:
    return 'T';//temperatureSensorLetter;
  case soilMoistureSensorCode:
    return 'M';//soilMoistureSensorLetter;
  }

  return 'U';
}

void refreshLCD()
{
  Serial.println("Refreshing LCD");
  
  bool idNotEmpty = currentId[0] > 0
    && currentId[1] > 0
    && currentId[2] > 0;
    
  if(millis() - previousMillis > interval) {
    previousMillis = millis();
    lcd.clear();
    lcd.setCursor(0,0);

    lcd.print("ID:");
    lcd.print(currentId[0]);
    lcd.print(".");
    lcd.print(currentId[1]);
    lcd.print(".");
    lcd.print(currentId[2]);
    lcd.setCursor(0,1);
    for (int i = 0; i < 3; i++)
    {
      lcd.print(getSensorLetter(sensorCodes[i]));
      lcd.print(":");   
      lcd.print(sensorValues[i]);
      lcd.print(" ");
    }
  }
}


