/*
jon rogers
05.05.2026

BASICS OF INTERACTIONS - the joystick drawing machine
In this tutorial you will learn how to read a two axis voltage divider joystick to control an 8x8 LED matrix

Preparation - make sure you've callibrated your joystick (values needed for MAX_JX and MAX_JY)

Using Arduino Nano BLE Sense 33
https://docs.arduino.cc/resources/pinouts/ABX00031-full-pinout.pdf?_gl=1*1rdm73z*_up*MQ..*_ga*MTgzMzUwOTczLjE3Nzc0NjEyMjI.*_ga_NEXN8H46L5*czE3Nzc0NjEyMjAkbzEkZzAkdDE3Nzc0NjEyMjAkajYwJGwwJGg5Mjg2MDU1MDE.

Adafruit's 8x8 1.2" matrix
https://www.adafruit.com/product/1049

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


#include "joystickToMatrix.h"

joystickToMatrix myGrid; 

void setup() {
  myGrid.setup(); 
  
}

void loop() {
 myGrid.update(); 
}