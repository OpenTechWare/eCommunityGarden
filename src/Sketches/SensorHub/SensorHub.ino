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

/*-----( Declare Constants and Pin Numbers )-----*/
#define NRF_CE_PIN   9
#define NRF_CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

/*-----( Declare objects )-----*/
RF24 radio(NRF_CE_PIN, NRF_CSN_PIN); // Create a Radio

/*-----( Declare Variables )-----*/
int data[5];

/* RTC */
uint8_t RTC_CE_PIN   = 5;
uint8_t RTC_IO_PIN   = 6;
uint8_t RTC_SCLK_PIN = 7;

/* Create buffers */
char buf[50];
char day[10];

/* Create a DS1302 object */
DS1302 rtc(RTC_CE_PIN, RTC_IO_PIN, RTC_SCLK_PIN);

const int lightSensorId = 1;
const int soilMoistureSensorId = 2;
const int temperatureSensorId = 3;

void setup()
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
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
      Serial.print(getSensorLabel(data[3]));
      Serial.print(":");     
      Serial.print(data[4]);
      Serial.print(";");
      //Serial.print("Tmp:");      
      //Serial.println(data[5]);
      Serial.println();
      
      //delay(100);
    }
  }
  else
  {    
      Serial.println("No radio available");
  }
  
  //delay(1000);
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

char* getSensorLabel(int sensorId)
{
  switch (sensorId)
  {
    case lightSensorId:
      return "Lt";
    case soilMoistureSensorId:
      return "Mst";
    case temperatureSensorId:
      return "Tmp";
  }
}
