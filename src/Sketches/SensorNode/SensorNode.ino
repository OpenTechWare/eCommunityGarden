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
#define JOYSTICK_X A0
#define JOYSTICK_Y A1

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int data[6];  // 2 element array holding Joystick readings

int id[] = {1, 1, 1};

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  int temperatureValue = analogRead(A3);
  temperatureValue = log(((10240000/temperatureValue) - 10000));
  temperatureValue = 1 / (0.001129148 + (0.000234125 * temperatureValue) + (0.0000000876741 * temperatureValue * temperatureValue * temperatureValue));
  temperatureValue = temperatureValue - 273.15;
  
  double lightValue = map(analogRead(A4), 0, 1023, 0, 100);
  double moistureValue = map(analogRead(A5), 1023, 0, 0, 100);
  
  data[0] = id[0];
  data[1] = id[1];
  data[2] = id[2];
  data[3] = lightValue;
  data[4] = moistureValue;
  data[5] = temperatureValue;
  
  radio.write( data, sizeof(data) );
}
