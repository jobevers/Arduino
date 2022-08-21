#include <FastLED.h>

uint16_t counter = 0;

const uint8_t N_LEDS = 8;
// This is what gets sent down the wire
CRGB led[N_LEDS];
// This is properly ordered fibers.
CRGB color[N_LEDS];
// Maps from the visual index (colors)
// the the wiring order (led)
int mapping[] = {
  0, 2, 4, 6, 1, 3, 5, 7
};
// redundant with the mapping (I could
// just swap it around) but it does
// lets us reset
//int transform_map[] = {0, 1, 2, 3, 4, 5, 6, 7};
int transform_map[] =   {0, 1, 2, 3, 4, 5, 6, 7};
//= {
//   5, 6, 7, 0, 1, 2, 3, 4
//};
// This is temp space for the rotate function
int _rotate[N_LEDS];
uint8_t R = 0;



uint8_t hue = 0;
// if true, every cycle - swap two pixels
boolean swap_mode = false;
// 0: no rotation
// 1: rotate clockwise  / / /
// 2: rotate countercolorise \ \ \ 
// 3: rotate clockwise, reverse /\/\/\/\
// 4: rotate counter clockwise, reverse \/\/\/\/
uint8_t rotation_mode = 3;


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

void loop() {
  set_color();
  if ((counter & 1) == 0) {
    update_transform_map();
  }
  //  if ((counter % 15) == 0) {
  //    swaps()  ;
  //  }
  if ((counter & 255) == 0) {
    for (int i = 0; i < N_LEDS; i++) {
      transform_map[i] = i;
    }
    R = 0;
    rotation_mode = random(5);
  }
  show();
  hue += 2;
  counter++;
  delay(62); // 1/16th of a second
}

void set_color() {
  for (int i = 0; i < N_LEDS; i++) {
    color[i] = CHSV(hue + 8 * i, 255, 255);
  }
}

void swaps() {
  int idx = random(N_LEDS);
  int next = (idx + 1) % N_LEDS;
  int tmp = transform_map[idx];
  transform_map[idx] = transform_map[next];
  transform_map[next] = tmp;
}

void update_transform_map() {
  switch (rotation_mode) {
    case 0:
      break;
    case 1:
      rotate(1);
      break;
    case 2:
      rotate(-1);
      break;
    case 3:
      if (R % N_LEDS == 0) {
        rotation_mode = 4;
        rotate(-1);
      } else {
        rotate(1);
      }
      break;
    case 4:
      if (R % N_LEDS == 0) {
        rotation_mode = 3;
        rotate(1);
      } else {
        rotate(-1);
      }
      break;
    default:
      break;
  }
  R++;
}

void rotate(int r) {
  for (int i = 0; i < N_LEDS; i++) {
    _rotate[i] = transform_map[(i + r + N_LEDS) % N_LEDS];
  }
  for (int i = 0; i < N_LEDS; i++) {
    transform_map[i] = _rotate[i];
  }
}


void show() {
  for (int i = 0; i < N_LEDS; i++) {
    led[mapping[transform_map[i]]] = color[i];
    //led[mapping[i]] = color[i];
  }
  FastLED.show();
}
// 256 / 4 = 64 / 4 = 16
