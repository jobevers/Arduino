//
// The idea here is have four LEDS with similar colors, each
// transitioning from max saturation & max value in hue A to a gray
// and then transition to max saturation & max value to hue B, where hue A and hue B
// are nearly complementary.  The transition should follow a triangle wave.
//  We don't do perfect complementary so
// that we are able to slowly cycle through the color wheel.
//
// Value and saturation need to be increased and reduced together. Want
// to avoid having a high value (white) and instead get grays when the saturation
// is low.
// 
// The timing of the changes also varies from pixel to pixel randomly.


#include <FastLED.h>

const int N_LEDS = 4;
const int STEPS = 128;
const int MIN_SATURATIOn = 20;
const int MAX_WAVE = 255;
// the array used to set the lights
CRGB led[N_LEDS];
// its easier to modify the HSV values though
uint8_t hues[N_LEDS];
uint8_t target_values[N_LEDS];
uint8_t steps[N_LEDS];

uint8_t random_target() {
  return random(16, 80);
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  // Set uninitialised LEDs to a faint grey
  memset8(led, 60, N_LEDS * sizeof(CRGB));

  randomSeed(analogRead(0));
  // pick a random hue to start with
  uint8_t start_hue = random(0, 256);
  hues[0] = start_hue - 15;
  hues[1] = start_hue - 5;
  hues[2] = start_hue + 5;
  hues[3] = start_hue + 15;

  for (int i = 0; i<N_LEDS; i++) {
    steps[i] = 0;
    target_values[i] = random_target();
  }

  const int output_pin = 6;
  FastLED.addLeds<WS2811, output_pin, RGB>(led, N_LEDS).setCorrection(Typical8mmPixel);
  FastLED.show();
 
}

// Returns a triangle wave between 255 and target
// with a period of STEPS * 2
//
// \    /\    /\
//  \  /  \  /  ...
//   \/    \/
//
uint8_t triangle_wave(uint8_t theta, uint8_t target) {
  float diff = float(MAX_WAVE - target);
  float step_size =  diff / STEPS;
  return uint8_t(abs((int(theta * step_size) % int(2 * diff)) - diff) + target);
}


uint8_t hue = 0;

// the loop function runs over and over again forever
void loop() {
  int index = random(0, N_LEDS);
  steps[index]++;
  uint8_t step = steps[index];
  uint8_t saturation = triangle_wave(step, 20);
  uint8_t value = triangle_wave(step, target_values[index]);
  if (step == STEPS) {
    // we are at the bottom of the triangle wave, its time to switch colors
    // The full complement would be 128, so, we're intentionally a little off
    hues[index] += 119;
  }
  if (step == 0) {
    // we are at the peak, pick a new target value to drop to
    target_values[index] = random_target();
  }

  led[index] = CHSV(hues[index], saturation, value);
  FastLED.show();
  delay(random(1, 6));
}
