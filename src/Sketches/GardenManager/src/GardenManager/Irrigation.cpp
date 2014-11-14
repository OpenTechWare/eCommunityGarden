#include <Arduino.h>

#include "MoistureSensor.h"

int threshold         = 20; // The minimum soil moisture level, before switching on the irrigation.

// LED pins
int wateringLedPin    = 15;

// Output pins
int wateringPin       = 9;
int wateringLowPin       = 8;

void checkIrrigation()
{
  int moistureValue = getMoistureValue();

  if (moistureValue < threshold)
  {
    digitalWrite(wateringPin, HIGH);
    digitalWrite(wateringLowPin, LOW);
    digitalWrite(wateringLedPin, HIGH);
  }
  else
  {
    digitalWrite(wateringPin, LOW);
    digitalWrite(wateringLowPin, HIGH);
    digitalWrite(wateringLedPin, LOW);
  }
}

void initIrrigation()
{
  pinMode(wateringPin, OUTPUT); 
  pinMode(wateringLedPin, OUTPUT);  
}
