#include <FastLED.h>

/*
Space Vikings
 */

 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int LED =  13;
#define LED_DATA_PIN 6
#define BUFFER_LENGTH 36 // >100LEDs unsupported
CRGB leds[BUFFER_LENGTH];
uint8_t base = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(LED_DATA_PIN, OUTPUT);
  // initialize the digital pin as an output.
  pinMode(LED, OUTPUT);

  // Set uninitialised LEDs to a faint grey
  memset8(leds, 10, BUFFER_LENGTH * sizeof(CRGB));

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  base = random(256);
  FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(leds, BUFFER_LENGTH).setCorrection(Typical8mmPixel);
  for (int i = 0; i < BUFFER_LENGTH; i++) {
      leds[i] = CHSV(base, random(256), random(128, 256));
  }
  FastLED.show();     
}

unsigned long next_time = 0;

// the loop routine runs over and over again forever:
void loop() {
  int pixel = random(BUFFER_LENGTH);
  if (millis() > next_time) {
    next_time += 5000;
    base += 130 + random(10);
  }
  leds[pixel] = CHSV(base, random(256), random(128, 256));
  FastLED.show();
  delay(28); // this is 1000 / 35 which means that on average each pixel changes once every second
}
