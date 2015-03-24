#include <Arduino.h>

#include <SD.h>
#include "Logger.h"

int chipSelect        = 4;
int sdCardPin         = 10;

void initSDCard()
{
  pinMode(sdCardPin, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    logError("Failed to write to SD card.");
    // don't do anything more:
    return;
  }
  
}
