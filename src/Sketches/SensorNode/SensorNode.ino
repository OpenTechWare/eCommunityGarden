/*
Code based on examples provided by:
http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
*/

/*
  How to add sensors...
  (The following numbers are used throughout the code to indicate where to modify it.)
  
  1) Add the sensor number (this must also be defined in the web.config file of the web application)

  2) Set the total sensors number
  
  3) Define the input pin of the sensor
      
  4) Get the sensor value during each loop
  
  6) Add the sensor value to the sensorValues array
  
  Note: Only a single sensor value is sent to the hub during each loop.
  while this seems counter-intuitive it allows virtually any number of sensors to be added.
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// nRF pins
#define CE_PIN   9
#define CSN_PIN 10

// ========== Start Modification Area

// !! Modify the following code to set a unique Id of the device

// Define the Id of the sensor node. This Id must be unique.
int id[] = {1, 1, 2};

// !! Modify the following code to change the nRF pipe/network to communicate on

// Define the nRF transmit pipe
// Devices using the same pipe will be able to communicate with each other.
// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL;

// !! Modify the following code when adding add new sensors

// Define the sensor numbers
const int lightSensorNumber = 1;
const int soilMoistureSensorNumber = 2;
const int temperatureSensorNumber = 3;
// 1) Add more sensor numbers here.
//    Define the sensor number in the web.config file of the web application
// const int [name]SensorNumber = 4;

// 2) Set this to the total number of sensors
const int totalSensors = 3;

// Define the sensor input pins
#define lightSensorPin A4
#define soilMoistureSensorPin A5
#define temperatureSensorPin A3
// 3) Add more sensors pin declarations here
// #define [name]SensorPin AX

// ========== End Modification Area

// Define the sensor numbers array
int sensorNumbers[totalSensors];

// Declare an array for the sensor values
int sensorValues[totalSensors];


// Create nRF radio object
RF24 radio(CE_PIN, CSN_PIN);

// Create an array to hold the data to be sent. (Don't modify this.)
int data[5];


// The current position in the list of sensors/values. It will increment automatically during each loop. (Don't modify this.)
int sensorPosition = 0;

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
}


void loop()
{
  // ========== Start Modification Area
  // !! Modify the following code when adding add new sensors
  
  // Get the sensor values...
  
  // Get the light sensor value
  double lightValue = map(analogRead(lightSensorPin), 0, 1023, 0, 100);
  
  // Get the soil moisture sensor value
  double soilMoistureValue = map(analogRead(soilMoistureSensorPin), 1023, 0, 0, 100);
  
  // Get the temperature sensor value
  int temperatureValue = analogRead(temperatureSensorPin);
  temperatureValue = log(((10240000/temperatureValue) - 10000));
  temperatureValue = 1 / (0.001129148 + (0.000234125 * temperatureValue) + (0.0000000876741 * temperatureValue * temperatureValue * temperatureValue));
  temperatureValue = temperatureValue - 273.15;
  
  // 4) Get the value of additional sensors here
  // double [name]Value = analogRead([name]SensorPin);
  // double [name]Value = map(analogRead([name]SensorPin), 0, 1023, 0, 100); // Converts analog input of 0-1023 to 0-100 (ie. percentage).
 
  // Set the sensor values...
  
  // Light sensor
  sensorNumbers[0] = lightSensorNumber;
  sensorValues[0] = lightValue;
  
  // Soil moisture sensor
  sensorNumbers[1] = soilMoistureSensorNumber;
  sensorValues[1] = soilMoistureValue;
  
  // Temperature sensor
  sensorNumbers[2] = temperatureSensorNumber;
  sensorValues[2] = temperatureValue;
  
  // 5) Set additional sensor values here
  // sensorNumbers[3] = [name]SensorNumber;
  // sensorValues[3] = [name]Value;
  
  // ========== End Modification Area
  // !! DO NOT modify the code below when adding sensors.
  
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
