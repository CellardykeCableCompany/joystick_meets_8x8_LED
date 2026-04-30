/*
jon rogers
26.04.2026

BASICS OF INTERACTIONS - the joystick.
In this tutorial you will learn how to read a two axis voltage divider joystick and calibrate it 
for use in arduino development. 


	voltage-divider  two-axis joystick - basics
  calibrate to find limits then use as you like

Using Arduino Nano BLE Sense 33
https://docs.arduino.cc/resources/pinouts/ABX00031-full-pinout.pdf?_gl=1*1rdm73z*_up*MQ..*_ga*MTgzMzUwOTczLjE3Nzc0NjEyMjI.*_ga_NEXN8H46L5*czE3Nzc0NjEyMjAkbzEkZzAkdDE3Nzc0NjEyMjAkajYwJGwwJGg5Mjg2MDU1MDE.


  Using this joystick from Pi Hut (part no 105757)]
  Data sheet https://cdn-shop.adafruit.com/product-files/5628/P5628_C17894-001-A+datasheet.png

  Pins (from top)
  P1 ADC1 (A0)
  P2 GND
  P3 Switch 
  P4 ADC2 (A1)
  P5 GND
  P6 VCC

  Joystick to Ardino Pins
  ADC 1 - A0 - 4
  ADC 2 - A1 - 5
  Switch - A2 - 6

  // from calibration 
  min_x = 397
  max_x = 1023

  min_y = 347
  max_y = 1023


 */


#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

//#define CALIB

#define MIN_JX 340 // calibrated joystick values
#define MAX_JX 1023
#define MIN_JY 340
#define MAX_JY 1023
#define MAX_MX 8
#define  MAX_MY 8

int ADC1 = A0; 
int ADC2 = A1; 
int S1 = A2; 

int vertical = 512;  // variable to store the value read
int horizontal = 512; 
int switch1= 0; 

// initiate joysick values as mid (512) min_x, max_x, min_y, max_y
int joystickLimits[4] = {512, 512, 512, 512};

void calibrate(int (&calib) [4]); 
void follow_joystick(int , int);

/* 8x8 Matrix */

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();



void setup() {
  Serial.begin(9600);           //  setup serial
  pinMode(S1, INPUT_PULLUP); 

  matrix.begin(0x70);  // pass in the address 0x70 for led backpack... 



}

void loop() {
 

#ifdef CALIB
 calibrate(joystickLimits);
 Serial.print(joystickLimits[2]); Serial.print("    "); Serial.print(joystickLimits[3]);
 Serial.println();
   delay(100);
#else

 //Serial.println("the joy begins... good luck!  ");
  int joyX; 
  int joyY; 
  joyX = analogRead(ADC1);
  joyY = analogRead(ADC2);
  //Serial.print("jx = "); Serial.print(jx);
  //Serial.println(); 

  follow_joystick(joyX, joyY);
  delay (100);
 #endif

}

/*   joystick    */
void calibrate(int (&calib) [4])
{

  // read adc - check limits
    // horizontal 
   horizontal = analogRead(ADC1);
  // Serial.print("horizontal = "); Serial.print(horizontal); Serial.println(); 
   if (horizontal < calib[0])
    calib[0] = horizontal; 

  Serial.print("vertical = "); Serial.print(vertical); Serial.println(); 
  if (horizontal>calib[1])
    calib[1] = horizontal; 

  // vertical
    vertical  = analogRead(ADC2);  
  if (vertical < calib[2])
    calib[2] = vertical; 
  if (vertical > calib[3])
    calib[3] = vertical; 

}

/* Matrix */

void follow_joystick(int jx, int jy)
{
   int x=0; 
   int y=0; 
  //Serial.println("in");
   x = map (jx, MIN_JX, MAX_JX, 0,  MAX_MX-1);
   y = map (jy, MIN_JY, MAX_JY, 0 , MAX_MY-1);
Serial.print(x); Serial.print(" "); Serial.print (y);
Serial.println(); 

  // draw 
  
  matrix.clear();
  matrix.drawPixel(x, y, LED_ON);
  matrix.writeDisplay();
  
  

}

