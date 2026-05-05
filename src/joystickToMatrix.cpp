#include "joystickToMatrix.h" 

// constructor - includes initial values of variables etc. 
joystickToMatrix::joystickToMatrix(){
// physicxal pins on the arduino nano
ADC1 = A0; 
ADC2 = A1; 
S1 = A2; 

vertical  = 512;  // variable to store the value read
horizontal  = 512; 
switch1 = 0;  
is_blink=false;

}

// public 

void joystickToMatrix::setup()
{
  Serial.begin(9600);           //  setup serial
  pinMode(S1, INPUT_PULLUP); 
  matrix.begin(0x70);  // pass in the address 0x70 for led backpack... 
  Serial.println("all set up");
}

void joystickToMatrix::update()
{
    int mX; 
    int mY; 
    int v; 
    unsigned long currentTime = millis();
  
    switch1=digitalRead(S1);
    // reverse switch value
    if (switch1==0) 
      v=1; 
    else
     v=0; 

    //Serial.println(switch1);
   
    read(mX, mY);
    draw(mX, mY,  v);
    // add blink state through timer
  
    if (currentTime - previousTime >= blinkInterval) {
        // blink it off where it was previously on
        //Serial.println(is_blink);
        previousTime = currentTime;
        is_blink=!is_blink; 
  }
 
   if (is_blink)
        blink(mX, mY);
    
matrix.writeDisplay();

}

void joystickToMatrix::calibrate(int (&calib) [4])
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


// private

void joystickToMatrix::read(int &x, int &y)
{
// using address reference as we want to change the value directly 

int  a1= analogRead(ADC1);
int  a2 = analogRead(ADC2);
Serial.print(a1); Serial.print ("  "); Serial.print(a2); Serial.println(); 

  x = map (a1, MIN_JX, MAX_JX, 0,  MAX_MX-1);
  y = map (a2, MIN_JY, MAX_JY, 0 , MAX_MY-1);
}

void joystickToMatrix::follow(int x, int y)
{
matrix.drawPixel(x, y, LED_ON);
  matrix.clear();
  matrix.writeDisplay();
  
}

void joystickToMatrix::draw(int x , int y, int v)
{
   led_grid[x][y]=v; 
   writeGrid(); 
}

void joystickToMatrix::blink(int x , int y)
{

    // change pixel on the matrix directly - thereby not changing the draw grid (to avoid ghost/shadow writing)
    if (led_grid[x][y]==1)
      matrix.drawPixel(x, y, LED_OFF); 
    else  
     matrix.drawPixel(x, y, LED_ON);


}
void joystickToMatrix::writeGrid ()
{
// write contents of the grid to the LED matrix

 // matrix.clear();
  
  for (int y=0; y<MAX_MY; y++){
    for (int x=0; x<MAX_MX; x++){
      if (led_grid[x][y]==1)
        matrix.drawPixel(x, y, LED_ON);
      else 
       matrix.drawPixel(x, y, LED_OFF);  
	}
  }
  
}


