#include <FastLED.h>
#include <ColorMap.h>


#define BUFFER_LENGTH 50 // >100LEDs unsupported
#define NUM_LEDS 50
#define LED_DATA_PIN 5
#define LED_DATA_PIN_SECOND_HALF 7
#define INPUT_PIN 4
#define LED_INDICATOR_PIN 9 // Hardwired on the board


CRGB led[BUFFER_LENGTH];
uint8_t offset[BUFFER_LENGTH];

class Pattern {
public:
    Pattern(){};
    void offsets(int, uint8_t[]);
};

void Pattern::offsets(int input, uint8_t offset[]) {
  int idx = 0;
  for (int fb=0; fb<2; fb++) { 
    for (int row=0; row<5; row++) {
      for (int col=0; col<5; col++) {
        if (!fb) {
          offset[idx++] = row;
        } else {
          offset[idx++] = 5 - row;
        }
      }
    }
  }
}


static int input = 0;
ColorMap cm;
Pattern pattern;

void initializeLeds()
{
    pinMode(LED_INDICATOR_PIN, OUTPUT);

    // Set uninitialised LEDs to a faint grey
    memset8(led, 255, BUFFER_LENGTH * sizeof(CRGB));

    FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(led, BUFFER_LENGTH).setCorrection(Typical8mmPixel);
    FastLED.show();

}

void setup() {
  initializeLeds();
  // put your setup code here, to run once:

}

void loop() {
  pattern.offsets(input, offset);
  for (int i=0; i<BUFFER_LENGTH; i++){
    led[i] = cm.color(offset[i] - input);
  }

  FastLED.show();
  delay(100);
  input += 1;
}
