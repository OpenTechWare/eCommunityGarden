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
#include <EEPROM.h>

// Buttons
#include <Button.h>

// LCD Display
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);


// nRF pins
#define CE_PIN   9
#define CSN_PIN 10

// ========== Start Modification Area

// !! Modify the following code to set a unique Id of the device

// Define the Id of the sensor node. This Id must be unique.
int id[] = {
  1, 1, 2};

// !! Modify the following code to change the nRF pipe/network to communicate on

// Define the nRF transmit pipe
// Devices using the same pipe will be able to communicate with each other.
// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL;

// !! Modify the following code when adding add new sensors

// Define the sensor numbers
const int lightSensorCode = 1;
const int soilMoistureSensorCode = 2;
const int temperatureSensorCode = 3;
// 1) Add more sensor numbers here.
//    Define the sensor number in the web.config file of the web application
// const int [name]SensorCode = 4;

// 2) Set this to the total number of sensors
const int totalSensors = 3;

// Define the sensor input pins
#define lightSensorPin A0
#define temperatureSensorPin A1
#define soilMoistureSensorPin A2
// 3) Add more sensors pin declarations here
// #define [name]SensorPin AX

// ========== End Modification Area

// Define the sensor numbers array
int sensorNumbers[totalSensors];

// Define the sensor codes array
int sensorCodes[totalSensors];

// Declare an array for the sensor values
int sensorValues[totalSensors];



// Declare an array for the sensor letters
char* sensorLetters[totalSensors];


// Create nRF radio object
RF24 radio(CE_PIN, CSN_PIN);

// Create an array to hold the data to be sent. (Don't modify this.)
int data[6];


// The current position in the list of sensors/values. It will increment automatically during each loop. (Don't modify this.)
int sensorPosition = 0;


Button idSelectButton = Button(2, &idSelectButtonHandler);
Button idUpButton = Button(3, &idUpButtonHandler);
Button idDownButton = Button(4, &idDownButtonHandler);

//Keep pointers to all the buttons in an array for simpler handling of many buttons
const int buttonAmount = 3;
Button* buttons[buttonAmount] = {
  &idSelectButton,
  &idUpButton,
  &idDownButton
};

int editIdPosition = -1;


void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
  
  // Start LCD display
  lcd.init();    
  lcd.backlight();

  loadId();
  //refreshLCD();
}


void loop()
{
  detectButton();

  // ========== Start Modification Area
  // !! Modify the following code when adding add new sensors

  // Get the sensor values...

  // Get the light sensor value
  double lightValue = map(analogRead(lightSensorPin), 0, 1023, 0, 100);

  // Get the soil moisture sensor value
  double soilMoistureValue = map(analogRead(soilMoistureSensorPin), 1023, 0, 0, 100);

  // Get the temperature sensor value
  int temperatureValue = getCelsius(analogRead(temperatureSensorPin));

  Serial.print("Light: ");
  Serial.println(lightValue);
  Serial.print("Moisture: ");
  Serial.println(soilMoistureValue);
  Serial.print("Temperature: ");
  Serial.println(temperatureValue);

  // 4) Get the value of additional sensors here
  // double [name]Value = analogRead([name]SensorPin);
  // double [name]Value = map(analogRead([name]SensorPin), 0, 1023, 0, 100); // Converts analog input of 0-1023 to 0-100 (ie. percentage).

  // Set the sensor values...

  // Light sensor
  sensorNumbers[0] = 1;
  sensorLetters[0] = "L";
  sensorCodes[0] = lightSensorCode;
  sensorValues[0] = lightValue;

  // Temperature sensor
  sensorNumbers[1] = 2;
  sensorLetters[1] = "T";
  sensorCodes[1] = temperatureSensorCode;
  sensorValues[1] = temperatureValue;
  
  // Soil moisture sensor
  sensorNumbers[2] = 3;
  sensorLetters[2] = "M";
  sensorCodes[2] = soilMoistureSensorCode;
  sensorValues[2] = soilMoistureValue;


  // 5) Set additional sensor values here
  // sensorNumbers[2] = 4;
  // sensorLetters[2] = "[letter]";
  // sensorCodes[3] = [name]SensorCode;
  // sensorValues[3] = [name]Value;

  // ========== End Modification Area
  // !! DO NOT modify the code below when adding sensors.

  // Add the Id and the sensor code/value for the current position.
  // Only one sensor value is sent per time.
  data[0] = id[0];
  data[1] = id[1];
  data[2] = id[2];
  data[3] = sensorNumbers[sensorPosition];
  data[4] = sensorCodes[sensorPosition];
  data[5] = sensorValues[sensorPosition];

  // Increment the sensor position to loop through each sensor value
  sensorPosition += 1;
  if (sensorPosition >= totalSensors)
    sensorPosition = 0;
    
  Serial.print("Id:");
  Serial.print(id[0]);
  Serial.print(".");
  Serial.print(id[1]);
  Serial.print(".");
  Serial.print(id[2]);
  Serial.print(";");
  Serial.print("#:");
  Serial.print(data[3]);
  Serial.print(";V:");
  Serial.print("S:");
  Serial.print(data[4]);
  Serial.print(";V:");
  Serial.print(data[5]);
  Serial.println(";");

  radio.write( data, sizeof(data) );
  
  //lazyRefreshLCD();
}

void detectButton()
{
  for (int i = 0; i < buttonAmount; i++){
    buttons[i]->read();
  }
}

void idSelectButtonHandler(Button* button)
{
  if (button->pressed){
    editIdPosition++;
    if (editIdPosition > 2)
      editIdPosition = -1;
  }
  else{

  }

  refreshLCD();
}

void idUpButtonHandler(Button* button)
{
  if (button->pressed){
    id[editIdPosition]++;
    if (id[editIdPosition] > 127)
      id[editIdPosition] = 0;
  }
  else{
    //Button released

  }

  saveId();
  refreshLCD();
}
void idDownButtonHandler(Button* button)
{
  if (button->pressed){
    id[editIdPosition]--;
    if (id[editIdPosition] < 1)
      id[editIdPosition] = 127;
  }
  else{
    //Button released

  }

  saveId();
  refreshLCD();
}

void refreshLCD()
{
  lcd.clear();
  lcd.print("Id: ");

  for (int i = 0; i < 3; i++)
  {
    if (editIdPosition == i)
      lcd.print("[");
    lcd.print(id[i]);
    if (editIdPosition == i)
      lcd.print("]");

    if (i < 2)
      lcd.print(".");
  }

  lcd.setCursor(0, 1);

  for (int i = 0; i < totalSensors; i++)
  {
    lcd.print(sensorLetters[i]);
    lcd.print(":");
    lcd.print(sensorValues[i]);
    lcd.print(" ");
  }
}

int skipRefreshCount = 0;

void lazyRefreshLCD()
{
  skipRefreshCount++;

  if (skipRefreshCount > 20)
  {
    refreshLCD();
    skipRefreshCount = 0;
  }
}

void saveId()
{
  for (int i = 0; i < 3; i++)
  {
    if (EEPROM.read(i) != id[i])
      EEPROM.write(i, id[i]);
  }
}

void loadId()
{
  int foundId[3];
  Serial.print("EEPROM ID:");  
  for (int i = 0; i < 3; i++)
  {
    foundId[i] = EEPROM.read(i);

    Serial.print(foundId[i]);

    if (i < 2)  
      Serial.print(".");

    if (foundId[i] > 0)
      id[i] = 0;
    else if (foundId[i] > 0)
      id[i] = foundId[i];

  }


}

double getCelsius(int analogInput) {
 double temp;
 temp = log(10000.0*((1024.0/analogInput-1))); 
//         =log(10000.0/(1024.0/analogInput-1)) // for pull-up configuration
 temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
 temp = temp - 273.15;            // Convert Kelvin to Celcius
 return temp;
}
