#include <FastLED.h>

#define BUFFER_LENGTH 8 // >100LEDs unsupported
#define LED_DATA_PIN 4

CRGB led[BUFFER_LENGTH];


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(LED_DATA_PIN, OUTPUT);
  // Set uninitialised LEDs to a faint grey
  memset8(led, 0, BUFFER_LENGTH * sizeof(CRGB));

  FastLED.addLeds<WS2812, LED_DATA_PIN, GRB>(led, BUFFER_LENGTH).setCorrection(TypicalSMD5050);
  FastLED.show();

}

uint8_t hue = 0;
uint8_t offset = 128;

void loop() {
  // put your main code here, to run repeatedly:
  // Turn the first led red for 1 second
  int i;
  int ledHue;
  for (i = 0; i < BUFFER_LENGTH; i++) {
    led[i] = CHSV(hue, 255, 255);
  }
  hue += 1;
  FastLED.show();
  delay(10);
}
