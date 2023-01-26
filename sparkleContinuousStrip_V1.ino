//based on sparkleExplore_V6
//basic extrapolation of just the sparkle functions (no background or text scrolling code included)
//best version for continuous sparkles on a strip

//added colorStyle to getRandom(colorStyle) and setSparkles(colorStyle) to allow for multiple color modes
//condensed code into a function. Isolated sparkle code from background code
//added gamma correction to sparkles
//improved color manipulation for sparkles
//set the color for the sparkle color in getRandom(uint8_t colorStyle)
//re-randomize the timing after each sparkle
//tweak timing by changing value ranges in getRandom()
//makes multiple sparkles using arrays
//using randomized timing for more organic sparkles
//A sketch to explore using timing with millis to create a sparkle effect
//sparkle is broken up into 3 parts: ramp up, hold, and trail off.
//Each part has variable timing

//for Circuit Playground Classic or Feather M4 Express or Metro M0
//written by David Crittenden 1/2023


#include <Adafruit_NeoPixel.h>
#define LED_PIN 12
#define LED_COUNT 64
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int i;//for loop manipulation
unsigned long currentMillis = millis();//holds the current time

//some variables for the SPARKLES
uint8_t colorStyle;//set with setSparkles(colorStyle) in the main loop

//set the color of the sparkles in getRandom(colorStyle)
uint8_t sparkleRed[LED_COUNT];
uint8_t sparkleGreen[LED_COUNT];
uint8_t sparkleBlue[LED_COUNT];

uint8_t stage[LED_COUNT];//variable to hold what stage the sparkle is in
int flashBright[LED_COUNT];//variable to hold light level of the sparkle

unsigned long rampUpPreviousMillis[LED_COUNT];
int rampUpInterval[LED_COUNT];//timing for the ramp up
uint8_t rampUpScaler[LED_COUNT];//number to add to flashBright each iteration loop

unsigned long holdPreviousMillis[LED_COUNT];
int holdInterval[LED_COUNT];//timing for the hold

unsigned long trailOffPreviousMillis[LED_COUNT];
int trailOffInterval[LED_COUNT];//timing for the trail off
uint8_t trailOffScaler[LED_COUNT];//number to subtract from flashBright each iteration loop

unsigned long pausePreviousMillis[LED_COUNT];
int pauseInterval[LED_COUNT];//timing for the pause between sparkles


//**************************************************************************************
void setup()
{
  strip.begin();
  strip.setBrightness(50);
  strip.show();

  //Serial.begin(9600);
  //while (!Serial);//wait for serial port to connect. Comment out if not using the serial monitor

  randomSeed(analogRead(0));//initialize the random function

  for (i = 0; i < LED_COUNT; i++)//fill the initial arrays with values
  {
    getRandom(colorStyle);//fill the arrays with values
    stage[i] = 4;//start with the pause stage to offset the beginning of all the sparkles
  }
}

void loop()
{
  currentMillis = millis();//check the current time

  strip.fill(0);//colorless background

  setSparkles(2);//0 for set color, 1 for random color, 2 purple, 3 red/green, 4 aqua

  strip.show();
}
//************************************************************************************

void setSparkles(uint8_t colorStyle)
{
  //iterate through all pixels
  for (i = 0; i < LED_COUNT; i++)
  {
    //quick ramp up STAGE 1
    if (stage[i] == 1)
    {
      if (currentMillis - rampUpPreviousMillis[i] > rampUpInterval[i])//is it time to advance?
      {
        flashBright[i] = flashBright[i] + rampUpScaler[i];//increace the brightness
        rampUpPreviousMillis[i] = currentMillis;//reset the timer
        holdPreviousMillis[i] = currentMillis;//reset the next timer
      }
      if (flashBright[i] >= 255)//as bright as it gets
      {
        flashBright[i] = 255;
        stage[i] = 2;//advance to the next stage
      }
    }

    //hold at full STAGE 2
    if (stage[i] == 2)
    {
      flashBright[i] = 255;//hold at max brightness

      if (currentMillis - holdPreviousMillis[i] > holdInterval[i])//is it time to advance?
      {
        stage[i] = 3;//advance to the next stage
        holdPreviousMillis[i] = currentMillis;//reset the timer
        trailOffPreviousMillis[i] = currentMillis;//reset the next timer
      }
    }

    //trail off STAGE 3
    if (stage[i] == 3)
    {
      if (currentMillis - trailOffPreviousMillis[i] > trailOffInterval[i])//is it time to advance?
      {
        flashBright[i] = flashBright[i] - trailOffScaler[i];//decreace the brightness
        trailOffPreviousMillis[i] = currentMillis;//reset the timer
        pausePreviousMillis[i] = currentMillis;//reset the next timer
      }
      if (flashBright[i] <= 0)//as dim as it gets
      {
        flashBright[i] = 0;
        stage[i] = 4;//advance to the next stage
      }
    }

    if (stage[i] == 4)//pause between sparkles
    {
      if (currentMillis - pausePreviousMillis[i] > pauseInterval[i])//is it time to advance?
      {
        pausePreviousMillis[i] = currentMillis;//reset the timer
        rampUpPreviousMillis[i] = currentMillis;//reset the next timer
        getRandom(colorStyle);//change the values in the array for this pixel
      }
    }
    //calculate pixel color brightness and do gamma correction
    int red = strip.gamma8(flashBright[i] * sparkleRed[i] / 255);
    int green = strip.gamma8(flashBright[i] * sparkleGreen[i] / 255);
    int blue = strip.gamma8(flashBright[i] * sparkleBlue[i] / 255);
    //set the pixel color value
    if (stage[i] != 4)strip.setPixelColor(i, red, green, blue);//don't sent data during the pause stage
  }
}

void getRandom(uint8_t colorStyle)//fill the arrays with randomized values
{
  rampUpInterval[i] = random(1, 40);
  rampUpScaler[i] = random(5, 10);//must be at least 1
  holdInterval[i] = random(1, 100);
  trailOffInterval[i] = random(1, 10);
  trailOffScaler[i] = random(5, 100);//must be at least 1
  pauseInterval[i] = random(500, 3000);

  stage[i] = 1;//return to step one

  if (colorStyle == 0)//set color
  {
    sparkleRed[i] = 252;
    sparkleGreen[i] = 245;
    sparkleBlue[i] = 75;
  }
  if (colorStyle == 1)//random color
  {
    sparkleRed[i] = random(256);
    sparkleGreen[i] = random(256);
    sparkleBlue[i] = random(256);
  }
  if (colorStyle == 2)//shades of purple
  {
    sparkleRed[i] = random(256);
    sparkleGreen[i] = 0;
    sparkleBlue[i] = random(256);
  }
  if (colorStyle == 3)//shades of red/green
  {
    sparkleRed[i] = random(256);
    sparkleGreen[i] = random(256);
    sparkleBlue[i] = 0;
  }
  if (colorStyle == 4)//shades of aqua
  {
    sparkleRed[i] = 0;
    sparkleGreen[i] = random(256);
    sparkleBlue[i] = random(256);
  }
}
