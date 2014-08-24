#include <Arduino.h>

// Light data variables
int lightSensorHigh        = 1000;
int lightSensorLow         = 0;
int lightValueHigh         = 100;
int lightValueLow          = 0;

int lightSensorPin = A3;

int getLightValue()
{
  int value = analogRead(lightSensorPin);    
  
  value = map(value, lightSensorHigh, lightSensorLow, lightValueLow, lightValueHigh);
  
  return value;
}
