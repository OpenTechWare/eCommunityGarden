/*
Code based on examples provided by:
http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
*/

/*
  How to add sensors...
  (The following numbers are used throughout the code to indicate where to modify it.)
  
  1) Add the sensor number (this must also be defined in the web.config file of the web application)
    Example:
      const int [name]SensorNumber = 4;
      
  2) Include the sensor number in the sensorNumbers array
    Example:
      int sensorNumbers[] = {
        ..,
        [name]SensorNumber
      };
  
  3) Increase the total sensors number
    Example:
      int totalSensors = 4;
      
  4) Define the sensor input pin
    Example:
      const int [name]SensorPin = A6;
      
  5) Get the sensor value during each loop
    Example:
      double [name]Value = map(analogRead([name]SensorPin), 0, 1023, 0, 100);
      
  6) Add the sensor value to the sensorValues array
    Example:
      sensorValues[3] = [name]Value;
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
// Create an array to hold the data to be sent. (Don't modify this.)
int data[5];

// Define the Id of the sensor node. This Id must be unique.
int id[] = {1, 1, 2};

// The current position in the list of sensors/values. (Don't modify this.)
int sensorPosition = 0;

// Define the sensor numbers.
// IMPORTANT: These must match those in the web.config file of the web application.
const int lightSensorNumber = 1;
const int soilMoistureSensorNumber = 2;
const int temperatureSensorNumber = 3;
// 1) Add more sensor numbers here

// Define the sensor numbers array.
// IMPORTANT: The position of these must match the position of the sensor values in the sensorValues array.
int sensorNumbers[] = {
  lightSensorNumber,
  soilMoistureSensorNumber,
  temperatureSensorNumber//,
  // 2) Add more sensor numbers here, but ensure they're defined in the web.config file as well.
  };

// Declare an array for the sensor values.
// IMPORTANT: The position of each value must match the position of the corresponding sensor numbers in the sensorNumbers array.
int totalSensors = 3; // 3) Increase this number to add new sensors.
int sensorValues[totalSensors];

// Define the sensor input pins
const int lightSensorPin = A4;
const int soilMoistureSensorPin = A5;
const int temperatureSensorPin = A3;
// 4) Add more sensors pin declarations here

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
}


void loop()
{
  
  // Get the light sensor value
  double lightValue = map(analogRead(lightSensorPin), 0, 1023, 0, 100);
  
  // Get the soil moisture sensor value
  double soilMoistureValue = map(analogRead(soilMoistureSensorPin), 1023, 0, 0, 100);
  
  // Get the temperature sensor value
  int temperatureValue = analogRead(temperatureSensorPin);
  temperatureValue = log(((10240000/temperatureValue) - 10000));
  temperatureValue = 1 / (0.001129148 + (0.000234125 * temperatureValue) + (0.0000000876741 * temperatureValue * temperatureValue * temperatureValue));
  temperatureValue = temperatureValue - 273.15;
  
  // 5) Get the value of any additional sensors here.
  
  // Set the sensor values. Each value must be in the same position as the corresponding label
  sensorValues[0] = lightValue;
  sensorValues[1] = soilMoistureValue;
  sensorValues[2] = temperatureValue;
  // 6) Add additional sensor values here. Ensure that each value has the corresponding sensor number in sensorNumbers array.
  
  // Add the Id and the sensor number/value for the current position.
  // Only one sensor value is sent per time.
  data[0] = id[0];
  data[1] = id[1];
  data[2] = id[2];
  data[3] = sensorNumbers[sensorPosition];
  data[4] = sensorValues[sensorPosition];
  
  // Increment the sensor position to loop through each sensor value
  sensorPosition += 1;
  if (sensorPosition >= totalSensors)
    sensorPosition = 0;
  
  radio.write( data, sizeof(data) );
}
