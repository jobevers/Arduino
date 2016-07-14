#include <FastLED.h>

const int ledBufferLength = 12;
const int LED_INDICATOR_PIN = 9;
const int LED_DATA_PIN = 6;

// the array used to set the lights
CRGB led[ledBufferLength];
int ledValues[ledBufferLength][3];
int remaining[ledBufferLength];


void initializeLeds()
{
    pinMode(LED_INDICATOR_PIN, OUTPUT);

    // Set uninitialised LEDs to a faint grey
    memset8(led, 60, ledBufferLength * sizeof(CRGB));

    FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(led, ledBufferLength).setCorrection(Typical8mmPixel);
    FastLED.show();
}

// the setup routine runs once when you press reset:
void setup() {
  randomSeed(analogRead(0));
  initializeLeds(); 
  for (int i=0; i<ledBufferLength; i++) {
    ledValues[i][0] = HUE_BLUE;
    ledValues[i][1] = 255;
    ledValues[i][2] = 255;
    remaining[i] = 0;
  }
  FastLED.setBrightness(64);
}


// For each LED, pick a random blue and then step towards it.
// Once that blue is hit, pick a different random blue, repeat
void loop() {
  digitalWrite(LED_INDICATOR_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  int step = 2;
  for (int i=0; i<ledBufferLength; i++) {
    if (remaining[i] == 0) {
      int maxDelta = 5;
      int upper = HUE_BLUE + 25;
      int lower = HUE_BLUE - 40;
      int value = ledValues[i][0];
      int diff = int(random(lower - value, upper - value));
      remaining[i] = diff / step;
    }
    if (remaining[i] > 0) {
      ledValues[i][0] += step;
      remaining[i]--;
    } else {
      ledValues[i][0] -= step;
      remaining[i]++;
    }
    led[i] = CHSV(ledValues[i][0], ledValues[i][1], ledValues[i][2]);
  }
  FastLED.show();
  digitalWrite(LED_INDICATOR_PIN, LOW);
  FastLED.delay(70);               // wait for a second
}
