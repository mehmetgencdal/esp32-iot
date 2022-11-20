#include <Wire.h>             // Oximetre
#include "MAX30105.h"         // Oximetre
#include "heartRate.h"        // Oximetre

#include <ESP32Servo.h>       // Buzzer

MAX30105 particleSensor;      // Oximetre

const int buzzer = 18;

const byte RATE_SIZE = 4;    //Increase this for more averaging 4
byte rates[RATE_SIZE];       //Array of heart rates
byte rateSpot = 0;           // Oximetre
long lastBeat = 0;           //Time at which the last beat occurred
float beatsPerMinute;        // Oximetre
int beatAvg;                 // Oximetre

int dd;                      // Buzzer

int voiceCardPin = 12;       // Voice Card
boolean value = 0;           // Voice Card


void setup()
{ 
  pinMode(buzzer, OUTPUT);
  pinMode(voiceCardPin, INPUT);
  
  dd=622;
  
  Serial.begin(115200);
  Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30102 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

void loop()
{
  Serial.println("*****");
  value = digitalRead(voiceCardPin);
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }


  if (irValue < 10000){
    Serial.print(" No finger?");
  } else {
      //Serial.print("IR=");
      //Serial.print(irValue);
      Serial.print("BPM= ");
      Serial.print(beatsPerMinute);
      Serial.print(", Avg BPM= ");
      Serial.print(beatAvg);

      Serial.print(", Voice: ");
      Serial.print(value);
      /*
      if(beatsPerMinute < 50 || value == 1){
          tone(buzzer, dd);
          delay(100);
          noTone(buzzer);
        }
      */
    }

  Serial.println();
}
