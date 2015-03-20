#ifndef FLOWMETER_CPP_
#define FLOWMETER_CPP_

#include "Arduino.h"

#include "FlowMeter.h"

byte flowMeterPin        = 2;

volatile byte flowPulseCount;  

// Flow meter variables

// Interrupt settings
byte sensorInterrupt       = 0;  // 0 = pin 2; 1 = pin 3

/**
 * Invoked by interrupt0 once per rotation of the hall-effect sensor. Interrupt
 * handlers should be kept as small as possible so they return quickly.
 */
void flowPulseCounter()
{
  // Increment the pulse counter
  flowPulseCount++;
}

void initFlowMeter()
{
  pinMode(flowMeterPin, INPUT);
  digitalWrite(flowMeterPin, HIGH);

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, flowPulseCounter, FALLING);
}

float getFlowRate()
{
  float flowRate;

  float calibrationFactor = 4.5;
  unsigned int flowMilliLitres;
  unsigned long totalMilliLitresA;
  unsigned long totalMilliLitresB;
  unsigned long oldTime;

  if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(sensorInterrupt);
    //lcd.setCursor(15, 0);
    //lcd.print("*");
    
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - oldTime)) * flowPulseCount) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitresA += flowMilliLitres;
    totalMilliLitresB += flowMilliLitres;
  
    // During testing it can be useful to output the literal pulse count value so you
    // can compare that and the calculated flow rate against the data sheets for the
    // flow sensor. Uncomment the following two lines to display the count value.
    //Serial.print(flowPulseCount, DEC);
    //Serial.print("  ");
    
    // Write the calculated value to the serial port. Because we want to output a
    // floating point value and print() can't handle floats we have to do some trickery
    // to output the whole number part, then a decimal point, then the fractional part.
    unsigned int frac;

    // Reset the pulse counter so we can start incrementing again
    flowPulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, flowPulseCounter, FALLING);
  }

  return flowRate;
}



#endif /* FLOWMETER_CPP_ */
