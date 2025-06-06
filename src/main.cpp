// Basic framework ******************************************************************************
#include <Arduino.h>
#include <FastLED.h>
#include "matrixMap_32x48_3pin.h"

#define HEIGHT 32 
#define WIDTH 48
#define NUM_LEDS ( WIDTH * HEIGHT )

#define NUM_SEGMENTS 3
#define NUM_LEDS_PER_SEGMENT 512

CRGB leds[NUM_LEDS];
uint16_t ledNum;

#define DATA_PIN_1 2
#define DATA_PIN_2 3
#define DATA_PIN_3 4

using namespace fl;

uint8_t SPEED = 5;
float speedfactor = SPEED/6;
uint8_t BRIGHTNESS = 40;

CRGB newcolor = CRGB::Black;

//*******************************************************************************************

void setup() {

  FastLED.addLeds<WS2812B, DATA_PIN_1, GRB>(leds, 0, NUM_LEDS_PER_SEGMENT)
    .setCorrection(TypicalLEDStrip);

  FastLED.addLeds<WS2812B, DATA_PIN_2, GRB>(leds, NUM_LEDS_PER_SEGMENT, NUM_LEDS_PER_SEGMENT)
    .setCorrection(TypicalLEDStrip);
   
  FastLED.addLeds<WS2812B, DATA_PIN_3, GRB>(leds, NUM_LEDS_PER_SEGMENT * 2, NUM_LEDS_PER_SEGMENT)
    .setCorrection(TypicalLEDStrip);

 FastLED.setBrightness(BRIGHTNESS);

 FastLED.clear();
 FastLED.show();

}

//*******************************************************************************************

void pride() {

  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 240, 255); 
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60); 
 
  uint16_t hue16 = sHue16; 
  uint16_t hueinc16 = beatsin88(113, 1, 3000 );    
  uint16_t ms = millis();  
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;     
  sPseudotime += deltams * msmultiplier*speedfactor;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;

  for( uint16_t i = 0 ; i < NUM_LEDS; i++ ) {
   
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
  
    newcolor = CHSV( hue8, sat8, bri8);
    ledNum = loc2indSerp[i];
  
    nblend( leds[ledNum], newcolor, 64);

  }

}

void loop() {
  pride(); 
  FastLED.show();
}
