/*****************************************************************************
  BH1790GLC.ino

 Copyright (c) 2016 ROHM Co.,Ltd.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
******************************************************************************/
#include <Wire.h>
#include <BH1790GLC.h>
#include <stdio.h>
#include <Arduino.h>
#include <ArduinoBLE.h>
#include <arduinoFFT.h>
BH1790GLC bh1790glc;
volatile bool timer_flg;
char str[100];

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service
 
// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
 
const int ledPin = LED_BUILTIN; // pin to use for the LED

void start_timer(void)
{		
  NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;  // Set the timer in Counter Mode
  NRF_TIMER2->TASKS_CLEAR = 1;               // clear the task first to be usable for later
	NRF_TIMER2->PRESCALER = 6;                             //Set prescaler. Higher number gives slower timer. Prescaler = 0 gives 16MHz timer
	NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;		 //Set counter to 16 bit resolution
	NRF_TIMER2->CC[0] = 6;                             //Set value for TIMER2 compare register 0
	NRF_TIMER2->CC[1] = 24000;                                 //Set value for TIMER2 compare register 1
		
  // Enable interrupt on Timer 2, both for CC[0] and CC[1] compare match events
	NRF_TIMER2->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos) | (TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos);
  NVIC_EnableIRQ(TIMER2_IRQn);
		
  NRF_TIMER2->TASKS_START = 1;               // Start TIMER2
}

void TIMER2_IRQHandler(void)
{
	if ((NRF_TIMER2->EVENTS_COMPARE[0] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE0_Msk) != 0))
  {
		NRF_TIMER2->EVENTS_COMPARE[0] = 0;           //Clear compare register 0 event	
		digitalWrite(LED_BUILTIN, 0);           //Set LED
    timer_flg = true;
  }
	
	if ((NRF_TIMER2->EVENTS_COMPARE[1] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE1_Msk) != 0))
  {
		NRF_TIMER2->EVENTS_COMPARE[1] = 0;           //Clear compare register 1 event
		digitalWrite(LED_BUILTIN, 1);
    timer_flg = true;         //Clear LED
  }
}

void setup() {
  
  //   // begin initialization
  // if (!BLE.begin()) {
  //   Serial.println("starting Bluetooth® Low Energy module failed!");
 
  //   while (1);
  // }

  //   // set advertised local name and service UUID:
  // BLE.setLocalName("LED");
  // BLE.setAdvertisedService(ledService);
 
  // // add the characteristic to the service
  // ledService.addCharacteristic(switchCharacteristic);
 
  // // add service
  // BLE.addService(ledService);
 
  // // set the initial value for the characeristic:
  // switchCharacteristic.writeValue(0);
 
  // // start advertising
  // BLE.advertise();
  
  pinMode(LED_BUILTIN, OUTPUT);

  byte rc;
  
  timer_flg = false;
  Serial.begin(115200);
  delay(10);
  while (!Serial);

  Wire.begin();
  Wire.setClock(400000L);

  pinMode(LED_BLUE, OUTPUT);
  
  rc = bh1790glc.init();

  if (rc != 0) {
    Serial.println("BH1790 initialize failed");
  } else {
    Serial.println("LEDON Data, LEDOFF Data");
  }

  pinMode(LED_GREEN, OUTPUT);
    
}

int main() {

  init();
  setup();



  while(1){

  // // listen for Bluetooth® Low Energy peripherals to connect:
  //   BLEDevice central = BLE.central();
  
  //   // if a central is connected to peripheral:
  //   if (central) {
  //     //Serial.print("Connected to central: ");
  //     // print the central's MAC address:
  //     //Serial.println(central.address());
  
  //     // while the central is still connected to peripheral:
  //   while (central.connected()) {
  //         //Serial.print("aaa");
  //         if (switchCharacteristic.written()) {
  //           if (switchCharacteristic.value()) {   
  //             //Serial.println("LED on");
  //             digitalWrite(LED_BUILTIN, LOW); // changed from HIGH to LOW       
  //           } else {                              
  //             //Serial.println(F("LED off"));
  //             digitalWrite(LED_BUILTIN, HIGH); // changed from LOW to HIGH     
  //           }
  //         }
  //       }
  
  //     // when the central disconnects, print it out:
  //     //Serial.print(F("Disconnected from central: "));
  //     //Serial.println(central.address());
  //   }    

  byte rc;
  unsigned short val[2];
 
  if (timer_flg) {
    rc = bh1790glc.get_val(val);
    if (rc == 0) {
      sprintf(str, ">LEDON:%d\n>LEDOFF:%d\n", -val[1], val[0]);
      digitalWrite(LED_BUILTIN,LOW);
      delay(200);
      digitalWrite(LED_BUILTIN,HIGH);
      delay(200);
    }
    timer_flg = false;
  }

  }

  return 0;

}