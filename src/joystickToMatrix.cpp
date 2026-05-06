#include "joystickToMatrix.h"

// constructor - includes initial values of variables etc.
joystickToMatrix::joystickToMatrix()
{
  // physicxal pins on the arduino nano
  ADC1 = A0;
  ADC2 = A1;
  S1 = A2;

  vertical = 512; // variable to store the value read
  horizontal = 512;
  switch1 = 0;
  is_blink = false;
  is_move = false;
  midPointJoyX = ((MAX_JX - MIN_JX) / 2) + MIN_JX + OFF_JX;
  midPointJoyY = ((MAX_JY - MIN_JY) / 2) + MIN_JY + OFF_JY;

  mX = 4;
  mY = 4;
}

// public

void joystickToMatrix::setup()
{
  Serial.begin(9600); //  setup serial
  pinMode(S1, INPUT_PULLUP);
  matrix.begin(0x70); // pass in the address 0x70 for led backpack...
  Serial.println("all set up");
}

void joystickToMatrix::update()
{

  int v;
  currentTimeBlink = millis();

  switch1 = digitalRead(S1);
  // reverse switch value
  if (switch1 == 0)
    v = 1;
  else
    v = 0;

  // Serial.println(switch1);

  read(mX, mY, JOY_MODE); //
  draw(mX, mY, v);
  blink(mX, mY);

  //  Serial.print(mX); Serial.print ("  "); Serial.print(mY); Serial.print("  "); Serial.print (switch1); Serial.println();

  matrix.writeDisplay();
}

void joystickToMatrix::calibrate(int (&calib)[4])
{
  // read adc - check limits
  // horizontal
  horizontal = analogRead(ADC1);
  // Serial.print("horizontal = "); Serial.print(horizontal); Serial.println();
  if (horizontal < calib[0])
    calib[0] = horizontal;

  // Serial.print("vertical = "); Serial.print(vertical); Serial.println();
  if (horizontal > calib[1])
    calib[1] = horizontal;

  // vertical
  vertical = analogRead(ADC2);
  if (vertical < calib[2])
    calib[2] = vertical;
  if (vertical > calib[3])
    calib[3] = vertical;
}

// private

void joystickToMatrix::read(int &x, int &y, int mode)
{
  /* using address reference as we want to change the value directly

  mode - 0 maps position of joystick directly to x,y
  mode - 1 moves x,y incrementally at predetermined speed ()

  */

  // Serial.print(x); Serial.print("   "); Serial.print(y); Serial.println();

  currentTimeJoy = millis();

  int a1 = 0;
  int a2 = 0;

  // bit of noise on the ADC - using this to average over 10 samples
  for (int s = 0; s < 10; s++)
  {
    a1 = a1 + analogRead(ADC1);
    a2 = a2 + analogRead(ADC2);
  }
  a1 = a1 / 10;
  a2 = a2 / 10;

  if (currentTimeJoy - previousTimeJoy >= joyInterval)
  {
    // blink it off where it was previously on
    // Serial.println(is_blink);
    previousTimeJoy = currentTimeJoy;
    is_move = true;
  }

  Serial.print(midPointJoyX);
  Serial.print(" ");
  Serial.print(a1);
  Serial.print(" ");
  Serial.print(midPointJoyY);
  Serial.print(" ");
  Serial.print(a2);
  Serial.println();

  switch (mode)
  {

  // map
  case 0:
    x = map(a1, MIN_JX, MAX_JX, 0, MAX_MX - 1);
    y = map(a2, MIN_JY, MAX_JY, 0, MAX_MY - 1);
    break;

  case 1:

    // increment
    // Serial.println(midPointJoyX);

    // if timer allows
    if (is_move)
    {
      if (a1 > (midPointJoyX + JOY_THRESH) && x < MAX_MX)
        x = x + 1;
      // Serial.println("left");
      if (a1 < (midPointJoyX - JOY_THRESH) && x > 0)
        x = x - 1;
      // Serial.println("right");
      if (a2 > (midPointJoyY + JOY_THRESH) && y < MAX_MY)
        y = y + 1;
      // Serial.println("up");
      if (a2 < (midPointJoyY - JOY_THRESH) && y > 0)
        y = y - 1;
      // Serial.println("down");
    }

    // x = 4;
    // y = 4;
    break;

  default:
    x = 4;
    y = 4;
    break;
  }

  x = constrain(x, 0, 7);
  y = constrain(y, 0, 7);
  is_move = false;
}

void joystickToMatrix::follow(int x, int y)
{
  matrix.drawPixel(x, y, LED_ON);
  matrix.clear();
  matrix.writeDisplay();
}

void joystickToMatrix::draw(int x, int y, int v)
{
  led_grid[x][y] = v;
  writeGrid();
}

void joystickToMatrix::blink(int x, int y)
{
  // blink the cursor to show where it is on the grid

  currentTimeBlink = millis();
  if (is_blink)
    blinkInterval = BLINK_ON_TIME;
  else
    blinkInterval = BLINK_OFF_TIME;

  if (currentTimeBlink - previousTimeBlink >= blinkInterval)
  {
    // blink it off where it was previously on
    // Serial.println(is_blink);
    previousTimeBlink = currentTimeBlink;
    is_blink = !is_blink;
  }

  if (is_blink)
  {
    // change pixel on the matrix directly - thereby not changing the draw grid (to avoid ghost/shadow writing)
    if (led_grid[x][y] == 1)
      matrix.drawPixel(x, y, LED_OFF);
    else
      matrix.drawPixel(x, y, LED_ON);
  }
}
void joystickToMatrix::writeGrid()
{
  // write contents of the grid to the LED matrix

  // matrix.clear();

  for (int y = 0; y < MAX_MY; y++)
  {
    for (int x = 0; x < MAX_MX; x++)
    {
      if (led_grid[x][y] == 1)
        matrix.drawPixel(x, y, LED_ON);
      else
        matrix.drawPixel(x, y, LED_OFF);
    }
  }
}
