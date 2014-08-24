#ifndef LOGGER_H_
#define LOGGER_H_

#include "Arduino.h"

void logStart();
void logFinish();
void logString(char value[]);
void logStringValue(char value[], char name[]);
void logIntValue(char name[], int value);
void logFloatValue(char name[], float value, int decimalPlaces);
void logError(char message[]);
void commitLogLine(char value[]);

#endif /* LOGGER_H_ */
