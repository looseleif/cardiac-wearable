#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"

MAX30105 particleSensor; // initialize MAX30102 with I2C

void setup()
{
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
  Serial.println("");
  Serial.println("MAX30102");
  Serial.println("");
  //delay(100);
  Serial.println("a");
  // Initialize sensor
//  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
//  {
//    Serial.println("MAX30105 was not found. Please check wiring/power. ");
//    while (1);
//  }
  particleSensor.begin(Wire, I2C_SPEED_FAST);
  Serial.println("b");

  byte ledBrightness = 70; //Options: 0=Off to 255=50mA
  byte sampleAverage = 1; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 69; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384

  Serial.println("c");
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  Serial.println("d");
}

void loop() {
  while (particleSensor.available()) {
      // read stored IR
      Serial.print(particleSensor.getFIFOIR());
      Serial.print(",");
      // read stored red
      Serial.println(particleSensor.getFIFORed());
      // read next set of samples
      particleSensor.nextSample();   

      Serial.print(" No finger?");
  }
}
