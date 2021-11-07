#include <FastLED.h>

const int N_LEDS = 32;
CRGB led[N_LEDS];
int hue = 0;
int mapping[] = {
   0,  1,  2,  3,  4,  5,  6,  7,
  -1, 14, 13, 12, 11, 10,  9,  8,
  16, 17, 18, 19, 20, 21, 22, 23,
  -1, 30, 29, 28, 27, 26, 25, 24
};
int N_SIDES = 4;
int SIDE_LENGTH = 8;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  pinMode(6, OUTPUT);  

  // Set uninitialised LEDs to a faint grey
  memset8(led, 255, N_LEDS * sizeof(CRGB));
//  for (int i=0; i<N_LEDS; i++) {
//    led[i] = CRGB::Blue;
//  }

  const int output_pin = 6;
  FastLED.addLeds<WS2812B, output_pin, GRB>(led, N_LEDS).setCorrection(TypicalSMD5050);
  FastLED.clear();
  FastLED.show();
 
}

void loop(){
  for (int j=0; j<N_SIDES; j++) {
    for (int i=0; i<SIDE_LENGTH; i++) {
      int idx = mapping[j*SIDE_LENGTH + i];
      if (idx == -1) {
        continue;
      }
      led[idx] = CHSV(hue + 8*i, 255, 255);
    }
    hue += 2;
  }
  //hue++;
  FastLED.show();
  delay(64);
}
// 256 / 4 = 64 / 4 = 16 
