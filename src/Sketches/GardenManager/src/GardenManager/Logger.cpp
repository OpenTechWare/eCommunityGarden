#include "Arduino.h"
#include "Logger.h"
#include "Conversion.h"
// Include SD card library
#include <SD.h>

char currentLogLine[250] = "";
char logFileName[] = "datalog.txt";


void logStart()
{  
  logString("D;");
}

void logFinish()
{
  Serial.println(currentLogLine);
  
  // Add a new line to the end of the current log line
  strcat(currentLogLine, "\r\n");
  
  // Write the current line of the log to the sd card
  commitLogLine(currentLogLine);
  
  // Clear the current log line ready for the next
  memset(currentLogLine,'\0',sizeof(currentLogLine));
}

void logError(char message[])
{
   // digitalWrite(errorPin, HIGH);
    Serial.print("Error: ");
    Serial.println(message);
  
}

void logIntValue(char name[], int value)
{
   char output[20];

   snprintf(output,20,"%d",value);
   
   logStringValue(name, output);
}

void logFloatValue(char name[], float value, int decimalPlaces)
{
   char stringValue[20];
   
   floatToString((char*)stringValue, value, decimalPlaces, 1, false);
   
   logStringValue(name, stringValue);
}

void logStringValue(char* name, char* value)
{
  strcat(currentLogLine, name);
  strcat(currentLogLine, ":");
  strcat(currentLogLine, value);
  strcat(currentLogLine, "; ");
}

void logString(char* value)
{
  strcat(currentLogLine, value);
}

void commitLogLine(char* value)
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.

  File dataFile = SD.open(logFileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(value);
    dataFile.close();
  }  
  // if the file isn't open, pop up an error:
  //else {
   // logError("error opening datalog.txt"); // TODO: This appears to be causing infinite loop
  //} 
}

