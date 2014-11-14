#include <Arduino.h>

#include "MoistureSensor.h"
#include "Logger.h"

int threshold         = 20; // The minimum soil moisture level, before switching on the irrigation.

// LED pins
int wateringLedPin    = 15;

// Output pins
int wateringPin       = 9;
int wateringAltPin       = 8;

void checkIrrigation()
{
  int moistureValue = getMoistureValue();

  if (moistureValue < threshold)
  {
    
    logStringValue("Irr", "On");
  
    digitalWrite(wateringPin, HIGH);
    digitalWrite(wateringAltPin, LOW);
    digitalWrite(wateringLedPin, HIGH);
  }
  else
  {
    logStringValue("Irr", "Off");
    
    digitalWrite(wateringPin, LOW);
    digitalWrite(wateringAltPin, HIGH);
    digitalWrite(wateringLedPin, LOW);
  }
}

void initIrrigation()
{
  pinMode(wateringPin, OUTPUT); 
  pinMode(wateringAltPin, OUTPUT); 
  pinMode(wateringLedPin, OUTPUT);  
}
