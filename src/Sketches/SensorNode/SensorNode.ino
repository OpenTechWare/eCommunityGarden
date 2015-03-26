/*
Code based on examples provided by:
http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
*/

/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

/*-----( Declare Variables )-----*/
int data[5];

int id[] = {1, 1, 2};

const int lightSensorId = 1;
const int soilMoistureSensorId = 2;
const int temperatureSensorId = 3;

int sensorNumber = 0;

int sensorValues[3];
int sensorLabels[] = { lightSensorId, soilMoistureSensorId, temperatureSensorId };

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
}


void loop()
{
  int temperatureValue = analogRead(A3);
  temperatureValue = log(((10240000/temperatureValue) - 10000));
  temperatureValue = 1 / (0.001129148 + (0.000234125 * temperatureValue) + (0.0000000876741 * temperatureValue * temperatureValue * temperatureValue));
  temperatureValue = temperatureValue - 273.15;
  
  double lightValue = map(analogRead(A4), 0, 1023, 0, 100);
  double moistureValue = map(analogRead(A5), 1023, 0, 0, 100);
  
  sensorValues[0] = lightValue;
  sensorValues[1] = moistureValue;
  sensorValues[2] = temperatureValue;
  
  data[0] = id[0];
  data[1] = id[1];
  data[2] = id[2];
  data[3] = sensorLabels[sensorNumber];
  data[4] = sensorValues[sensorNumber];
  
  sensorNumber += 1;
  if (sensorNumber >= 3)
    sensorNumber = 0;
  
  radio.write( data, sizeof(data) );
}
