#include "Logger.h"
#include <DHT11.h>

int dht11Pin = 3;
float humidityValue = 0;
float temperatureValue = 0;

DHT11 dht11(dht11Pin);

void checkHumidityTemperatureValues()
{
  int err;
  if((err=dht11.read(humidityValue, temperatureValue))==0)
  {
  }
  else
  {
    logError("DHT 11 error");
  }
}

float getHumidityValue()
{
  return humidityValue;
}

float getTemperatureValue()
{
  return temperatureValue;
}
