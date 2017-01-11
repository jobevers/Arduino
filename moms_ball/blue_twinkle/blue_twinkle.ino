#include <FastLED.h>

const int ledBufferLength = 24;
const int LED_INDICATOR_PIN = 9;
const int LED_DATA_PIN = 6;

// the array used to set the lights
CRGB led[ledBufferLength];
int ledValues[ledBufferLength][3];
int remaining[ledBufferLength];

// bounds on the allowed color
int upper = HUE_BLUE + 25;
int lower = HUE_BLUE - 40;

//
int nWhites = 3; 
int whites[] = {0, 5, 10};
int whiteCountPrimary[] = {5, 7, 9};
int whiteCountSecondary[] = {3, 5, 3};


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

int findWhite(int idx) {
  for (int i=0; i<nWhites; i++) {
    if (whites[i] == idx) {
      return i;
    }
  }
  return -1;
}

void showWhite(int idx, int whiteIdx) {
  if (whiteCountPrimary[whiteIdx] % 2) {
    led[idx] = CHSV(ledValues[idx][0], ledValues[idx][1], ledValues[idx][2]);
  } else {
    led[idx] = CRGB(255, 255, 255);
  }
  whiteCountSecondary[whiteIdx]--;
  if (whiteCountSecondary[whiteIdx] == 0) {
    whiteCountSecondary[whiteIdx] = random(3, 10);
    whiteCountPrimary[whiteIdx]--;
    if (whiteCountPrimary[whiteIdx] == 0) {
        whites[whiteIdx] = random(0, ledBufferLength);
        whiteCountPrimary[whiteIdx] = random(3, 10);
    }
  }
}

// For each LED, pick a random blue and then step towards it.
// Once that blue is hit, pick a different random blue, repeat
void loop() {
  digitalWrite(LED_INDICATOR_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  int step = 2;
  for (int i=0; i<ledBufferLength; i++) {
    int whiteIdx = findWhite(i);
    if (whiteIdx >= 0) {
      showWhite(i, whiteIdx);
    } else {
      if (remaining[i] == 0) {
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
  }
  FastLED.show();
  digitalWrite(LED_INDICATOR_PIN, LOW);
  FastLED.delay(70);
}
