#pragma once // used to stop header loading this headerfile more than one... 
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

//#define CALIB
//#define DEBUG

#define MIN_JX 340 // calibrated joystick values
#define MAX_JX 1023
#define MIN_JY 340
#define MAX_JY 1023
#define MAX_MX 8
#define  MAX_MY 8


class joystickToMatrix {
  public:
    joystickToMatrix();        // constructor
    void setup();
    void update();             // call this every loop to update the matrix 
    void calibrate();           // only needed when calibrating (to find the #define values MAX JX...etc)

    private: 

    int ADC1, ADC2, S1; 

    // matrix  - the 8x8 LED from adafruit
    Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

    // the grid - where i store the values of the matrix in
    int led_grid[MAX_MX][MAX_MY] = {0}; // initialise grid with zero ( 0 = off; 1 = on) 

    // timer
    const unsigned long blinkInterval = 500; // set blink in milliseconds 
    unsigned long previousTime = 0;

    int vertical;    // variable to store the value read
    int horizontal; 
    int switch1; 

    // joystick 
    bool is_blink; 
    // initiate joysick values as mid (512) min_x, max_x, min_y, max_y
    int joystickLimits[4] = {512, 512, 512, 512};

    void read (int &, int &);
    void follow(int , int);
    void draw(int , int, int);
    void blink(int , int);
    void writeGrid ();
    void calibrate(int (&calib) [4]); 
}; 