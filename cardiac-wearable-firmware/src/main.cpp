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

BH1790GLC bh1790glc;
volatile bool timer_flg;
char str[100];
 
const int ledPin = LED_BUILTIN; // pin to use for the LED

void setup() {
  
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