#include <stdio.h>
#include <string.h>
#include <DS1302.h>
#include <Arduino.h>

#include "Time.h"

uint8_t rtcCEPin      = 5; // RTC
uint8_t rtcIOPin      = 6; // RTC
uint8_t rtcSCLKPin    = 7; // RTC

// Create a DS1302 object for RTC
DS1302 rtc(rtcCEPin, rtcIOPin, rtcSCLKPin);

char* getTime()
{
  char dateTime[60];

  /* Get the current time and date from the chip */
  Time t = rtc.time();

   
//   rtc.minutes(38);
   
//   rtc.hour(13);

//   rtc.month(3);
   
//   rtc.date(22);
   
//   rtc.year(2015);
  /* Format the time and date */
  snprintf(dateTime, sizeof(dateTime), "%04d-%02d-%02d %02d:%02d:%02d",
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  return dateTime;
}
