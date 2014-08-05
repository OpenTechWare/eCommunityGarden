
// SD card
#include <SD.h>

// Temperature/humidity sensor
#include <DHT11.h>

// RTC (real time clock) 
#include <stdio.h>
#include <string.h>
#include <DS1302.h>

/* Set the appropriate digital I/O pin connections for RTC */
uint8_t CE_PIN   = 5;
uint8_t IO_PIN   = 6;
uint8_t SCLK_PIN = 7;

/* Create buffers for RTC */
char buf[50];
char day[10];

int powerLedPin = 9;

/* Create a DS1302 object for RTC */
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);

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

int delayTime = 2000;

char logFileName[ ] = "datalog.txt";

DHT11 dht11(humidityTempPin); 

String dateTime = "";

void getTime()
{
  /* Get the current time and date from the chip */
  Time t = rtc.time();

  /* Name the day of the week */
  memset(day, 0, sizeof(day));  /* clear day buffer */
  switch (t.day) {
    case 1:
      strcpy(day, "Sunday");
      break;
    case 2:
      strcpy(day, "Monday");
      break;
    case 3:
      strcpy(day, "Tuesday");
      break;
    case 4:
      strcpy(day, "Wednesday");
      break;
    case 5:
      strcpy(day, "Thursday");
      break;
    case 6:
      strcpy(day, "Friday");
      break;
    case 7:
      strcpy(day, "Saturday");
      break;
  }

  /* Format the time and date and insert into the temporary buffer */
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day,
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  /* Print the formatted string to serial so we can see the time */
  dateTime = buf;
}

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
  getTime();
  
  readMoisture();
  
  readLight();
  
  readHumidityTemp();
  
  logStart();
  logData("DateTime", dateTime);
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

void logData(String name, int value)
{
   char output[50];
   
   snprintf(output,50,"%d",value);
   
   logData(name, output);
}

void logData(String name, String value)
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
  logString("\r\n");
  
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
  
  moistureSensorValue = map(moistureSensorValue, moistureSensorHigh, moistureSensorLow, moistureValueLow, moistureValueHigh);
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
