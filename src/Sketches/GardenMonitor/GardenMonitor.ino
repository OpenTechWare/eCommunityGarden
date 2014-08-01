
#include <SD.h>
#include <DHT11.h>

int powerLedPin = 9;

int moistureSensorPin = A4; 
int moistureSensorValue = 0;  // variable to store the value coming from the sensor
int moistureSensorHigh = 1030;
int moistureSensorLow = 350;
int moistureValueHigh = 100;
int moistureValueLow = 0;

int lightSensorPin = A3; 
int lightSensorValue = 0;
int lightSensorHigh = 1000;
int lightSensorLow = 0;
int lightValueHigh = 100;
int lightValueLow = 0;

int humidityTempPin = 3;
float temperature = 0;
float humidity = 0;

int ledPin = 13;      // select the pin for the LED
int errorPin = 8;

int sdCardPin = 10;

int threshold = 400;

int chipSelect = 4;

int delayTime = 1000;

char logFileName[ ] = "datalog.txt";

DHT11 dht11(humidityTempPin); 

// Store the current log line as a variable then write it to the log when it's finished
// When written directly to the log then there's a risk of log file corruption if the arduino is turned off part way through a line
String currentLogLine = "";

void setup() {
  Serial.begin(9600);
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);  
  
  // declare the ledPin as an OUTPUT:
  pinMode(powerLedPin, OUTPUT);  
  pinMode(errorPin, OUTPUT);
  
  // turn the power LED on
  digitalWrite(powerLedPin, HIGH);
    
  initializeSDCard();
}

void loop() {
  readMoisture();
  
  readLight();
  
  readHumidityTemp();
  
  logStart();
  logData("Moisture", moistureSensorValue);
  logData("Light", lightSensorValue);
  logData("Humidity", humidity);
  logData("Temperature", temperature);
  logFinish();
  
  if (moistureSensorValue < threshold) 
  {
   // Serial.println("Moisture level: high");
    //digitalWrite(13, LOW);
  }
  else
  {
    //Serial.println("Moisture level: low");
    //digitalWrite(13, HIGH);
  }
  
  delay(delayTime);
}

void initializeSDCard()
{
  pinMode(sdCardPin, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    logError("Failed to write to SD card.");
    // don't do anything more:
    return;
  }
  
}

void logString(String value)
{
  currentLogLine += value;
}

void commitLogLine(String value)
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(logFileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(value);
    dataFile.close();
    // print to the serial port too:
    Serial.print(value);
  }  
  // if the file isn't open, pop up an error:
  else {
    logError("error opening datalog.txt");
  } 
}

void logData(char name[], int value)
{
  // make a string for assembling the data to log:
  String dataString = "";

  dataString += name;
   dataString += ": ";
  dataString += String(value);
  dataString += ";";
  dataString += " ";

  logString(dataString);
}

void logFinish()
{
  // Add a new line to the end of the current log line
  logString("\n");
  
  // Write the current line of the log to the sd card
  commitLogLine(currentLogLine);
  
  // Clear the current log line ready for the next
  currentLogLine = "";
}

void logStart()
{
  // Clear the current log line ready for the next (shouldn't need this as it's cleared in logFinish but it's just an added precaution)
  currentLogLine = "";
  
  logString("Data; ");
}

void readMoisture()
{
  // read the value from the sensor:
  moistureSensorValue = analogRead(moistureSensorPin);    
  
  moistureSensorValue = map(moistureSensorValue, mositureSensorHigh, moistureSensorLow, moistureValueLow, moistureValueHigh);
}

void readLight()
{
  // Read the light sensor value
  lightSensorValue = analogRead(lightSensorPin);    
  
  // Map the 
  lightSensorValue = map(lightSensorValue, lightSensorHigh, lightSensorLow, lightValueLow, lightValueHigh);
}

void readHumidityTemp()
{
  
  int err;
  if((err=dht11.read(humidity, temperature))==0)
  {
  }
  else
  {
    logError((char*)err);  
  }
}

void logError(char message[])
{
  digitalWrite(errorPin, HIGH);
    Serial.print("Error: ");
    Serial.println(String(message));
  
}
