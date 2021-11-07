#include <FastLED.h>

/*
Space Vikings
 */

 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int LED =  13;
#define LED_DATA_PIN 6
#define UV_LED_DATA_PIN 5
#define BUFFER_LENGTH 6 // >100LEDs unsupported
CRGB leds[BUFFER_LENGTH];
uint8_t brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
int show_uv = 0;
int color = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(LED_DATA_PIN, OUTPUT);
  pinMode(UV_LED_DATA_PIN, OUTPUT);
  // initialize the digital pin as an output.
  pinMode(LED, OUTPUT);

  // Set uninitialised LEDs to a faint grey
  memset8(leds, 10, BUFFER_LENGTH * sizeof(CRGB));

  FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(leds, BUFFER_LENGTH).setCorrection(Typical8mmPixel);
  FastLED.show();     
}


// the loop routine runs over and over again forever:
void loop() {
  if (show_uv) {
    analogWrite(UV_LED_DATA_PIN, brightness);
  } else {
    CRGB pixel;
    if (color == 0) {
      pixel = CRGB(255, 0, 255); 
    } else if (color == 1) {
      pixel = CRGB(0, 255, 255); 
    } else if (color == 2) {
      pixel = CRGB(0, 0, 255); 
    }
    for (int i = 0; i < BUFFER_LENGTH; i++) {
        leds[i] = pixel;
    }
    FastLED.setBrightness(brightness);
  }
  brightness += fadeAmount;
  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  if (brightness <= 0) {
    if (show_uv) {
      show_uv = 0;
      color = (color + 1) % 3;
    } else {
      show_uv = 1;
    }
  }
  FastLED.show();
  FastLED.delay(20);
}
