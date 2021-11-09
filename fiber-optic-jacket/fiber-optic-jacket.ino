#include <FastLED.h>

const uint8_t N_LEDS = 8;
// This is what gets sent down the wire
CRGB led[N_LEDS];
// This is properly ordered fibers.
CRGB color[N_LEDS];
// Maps from the visual index (colors)
// the the wiring order (led)
uint8_t mapping[] = {
  0, 2, 4, 6, 1, 3, 5, 7
};


uint8_t hue = 0;
// if true, every cycle - swap two pixels
boolean swap_mode = false;
// 0: no rotation
// 1: rotate clockwise  / / / 
// 2: rotate countercolorise \ \ \ 
// 3: rotate clockwise, reverse /\/\/\/\
// 4: rotate counter clockwise, reverse \/\/\/\/
uint8_t rotation_mode = 0;


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
int n_lit_leds = 8;
int lit_leds[] = {0, 1, 2, 3, 4, 5, 6, 7};
void loop(){
  for (int i=0; i<n_lit_leds; i++) {
    //color[i] = CHSV(hue + 8*i, 255, 255);
    int cur_led = lit_leds[i];
    int idx = mapping[cur_led];
    led[idx] = CHSV(hue + 8*i, 255, 255);
    //lit_leds[i] = (lit_leds[i] + 1) % N_LEDS;
  }
  hue += 2;
  //hue++;
  FastLED.show();
  delay(62); // 1/16th of a second
}


void show() {
  for (int i=0; i<N_LEDS; i++) {
    led[mapping[i]] = color[i];  
  }
  FastLED.show();
}
// 256 / 4 = 64 / 4 = 16 
