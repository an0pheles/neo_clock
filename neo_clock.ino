#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            4
#define NUMPIXELS      24

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastTime;
int seconds; 
int minutes; 
int hours; 

void setup()
{
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(190);
  lastTime = millis();
  seconds = 0; 
  minutes = 4; 
  hours = 4; 
}

void loop() 
{
  int currentTime = millis();
  if (currentTime - lastTime > 1000)
  {
    if (seconds == 60)
    {
      seconds = 0;
      if (minutes == 60)
      {
        minutes = 0;
        if (hours == 12)
        {
          hours = 0;
        } else 
        {
          hours++;
        }
      } else
      {
        minutes++;
      }
    } else 
    {
      seconds++; 
    }
    lastTime = currentTime; 
    setLeds();
  }
}

void setLeds()
{
  int minuteLed = (seconds / 5) * 2;
  for (int i=0; i<NUMPIXELS; i++)
  {
    if (i==minuteLed)
    {
      pixels.setPixelColor(i, 255, 0, 255);
    } else
    {
      pixels.setPixelColor(i, 1, 3, 5);
    }
  }
  pixels.show();
}

