#include <Arduino.h>
#include "Logger.h"

// Light data variables
int lightSensorHigh        = 1000;
int lightSensorLow         = 0;
int lightValueHigh         = 100;
int lightValueLow          = 0;

// Declare the pin that measures light levels
int lightSensorPin = A3;

// Declare the pin that controls grow lights
int lightControlPin = 8;

// Declare the minimum light level, at which lower levels will turn on grow lights
int lightControlThreshold = 80;

void initLight()
{
  pinMode(lightControlPin, OUTPUT);
}
  
int getLightValue()
{
  int value = analogRead(lightSensorPin);    
  
  value = map(value, lightSensorHigh, lightSensorLow, lightValueLow, lightValueHigh);
  
  int lightOutputValue = map(value, lightControlThreshold, 0, 0, 255);
  
  if (lightOutputValue < 0)
    lightOutputValue == 0;
  
  logIntValue("LtOut", lightOutputValue);
  
  // If light levels are low, turn on the grow lights
  if (value < lightControlThreshold)
    analogWrite(lightControlPin, lightOutputValue);
  else
    analogWrite(lightControlPin, 0);
  
  return value;
}
