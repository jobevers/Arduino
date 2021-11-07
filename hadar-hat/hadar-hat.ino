#include <FastLED.h>

const int N_LEDS = 5;
CRGB led[N_LEDS];
int hue = 0;
int mapping[] = {
    4, 3, 0, 1, 2
};

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

// 0: 2
// 1: 3
// 2: 4
// 3: 1
// 4: 0
int n_lit_leds = 5;
int lit_leds[] = {0, 1, 2, 3, 4};
void loop(){
  for (int i=0; i<N_LEDS; i++) {
    led[i] = CRGB::Black;  
  }
  for (int i=0; i<n_lit_leds; i++) {
    int cur_led = lit_leds[i];
    int idx = mapping[cur_led];
    led[idx] = CHSV(hue + 8*i, 255, 255);
    lit_leds[i] = (lit_leds[i] + 1) % N_LEDS;
  }
  hue += 2;
  //hue++;
  FastLED.show();
  delay(50);
}
// 256 / 4 = 64 / 4 = 16 
