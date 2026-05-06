#pragma once // used to stop header loading this headerfile more than one...
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

// #define CALIB
// #define DEBUG

#define MIN_JX 400 // calibrated joystick values
#define MAX_JX 1023
#define MIN_JY 400
#define MAX_JY 1023
#define OFF_JX 100;
#define OFF_JY 100;
#define MAX_MX 8
#define MAX_MY 8
#define JOY_MODE 1        // way the joystick changes x, y values
#define JOY_THRESH 100    // threhold joystick has to move before increment happens (buffer zone in middle)
#define BLINK_ON_TIME 200 // using this as an equal blink on/off time is visually disturbing
#define BLINK_OFF_TIME 1

class joystickToMatrix
{
public:
  joystickToMatrix(); // constructor
  void setup();
  void update();    // call this every loop to update the matrix
  void calibrate(); // only needed when calibrating (to find the #define values MAX JX...etc)

private:
  int ADC1, ADC2, S1;
  long int midPointJoyX, midPointJoyY;

  // matrix  - the 8x8 LED from adafruit
  Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

  // the grid - where i store the values of the matrix in
  int led_grid[MAX_MX][MAX_MY] = {0}; // initialise grid with zero ( 0 = off; 1 = on)

  // timers
  // note i'm using const unsigned long / int here - i could have used a #define...
  // set blink in milliseconds
  unsigned long previousTimeBlink = 0;
  unsigned long currentTimeBlink;
  unsigned int blinkInterval;

  unsigned long currentTimeJoy; // maybe same as currentTimeBlink - can swap out for generic "time" variable...
  unsigned long previousTimeJoy;
  const unsigned int joyInterval = 100; // speed at which the joystick moves...

  int vertical; // variable to store the value read
  int horizontal;
  int switch1;

  // matrix
  bool is_blink;

  // joystick
  bool is_move;
  // cursor position on matrix
  int mX;
  int mY;

  // initiate joysick values as mid (512) min_x, max_x, min_y, max_y
  int joystickLimits[4] = {512, 512, 512, 512};

  void read(int &, int &, int);
  void follow(int, int);
  void draw(int, int, int);
  void blink(int, int);
  void writeGrid();
  void calibrate(int (&calib)[4]);
};