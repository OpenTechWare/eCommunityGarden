#include "Arduino.h"
#include "Logger.h"
#include "Conversion.h"

// Moisture data variables
int moistureValue    = 0;
int moistureSensorHigh     = 1023;
int moistureSensorLow      = 0;
int moistureValueHigh      = 100;
int moistureValueLow       = 0;

int moistureSensorPin = A4;

int getMoistureValue()
{
  moistureValue = analogRead(moistureSensorPin);    
  
  moistureValue = map(moistureValue, moistureSensorHigh, moistureSensorLow, moistureValueLow, moistureValueHigh);
  
  return moistureValue;
}
