#include <FastLED.h>

#define BUFFER_LENGTH 50
#define LED_INDICATOR_PIN LED_BUILTIN
#define LED_DATA_PIN 4

CRGB led[BUFFER_LENGTH];
CRGB prev_led[BUFFER_LENGTH];
CRGB target_led[BUFFER_LENGTH];
CRGB next_led[BUFFER_LENGTH];

// 
unsigned long switch_time = 0;
// updated every frame
unsigned long show_time = 0;
uint8_t hue = 0;

void initializeLeds()
{
  pinMode(LED_INDICATOR_PIN, OUTPUT);

  // Set uninitialised LEDs to a faint grey
  memset8(led, 255, BUFFER_LENGTH * sizeof(CRGB));

  FastLED.addLeds<WS2811, LED_DATA_PIN, GRB>(led, BUFFER_LENGTH).setCorrection(Typical8mmPixel);
  FastLED.show();
  for (int i = 0; i < BUFFER_LENGTH; i++) {
    prev_led[i] = led[i];
    target_led[i] = CHSV(hue, 255, 255);
  }
}

int idx = 0;

void setup() {
  initializeLeds();
  Serial.begin(9600);
}

uint8_t bytes_expected = 0xFF;
// 0 = even pixels
// 1 = odd pixels
uint8_t odd_or_even = 0xFF;
uint8_t bytes_recieved = 0xFF;
int count = 0;

void loop() {
  unsigned long now = millis();
  // Code here to populate next_led
  // Should read from the serial port
  for (int i = 0; i < BUFFER_LENGTH; i++) {
      next[i] = CHSV(hue + i * 5, 255, 255);
  }
  if (now >= switch_time) {
    for (int i = 0; i < BUFFER_LENGTH; i++) {
      prev_led[i] = target_led[i];
      target_led[i] = next_led[i];
    }
    hue += 127;
    switch_time += 1000;
    Serial.print("Hue "); Serial.println(hue);
  }
  // This scale calculation is a stand-in
  // for the actual scale calculation
  // which would be
  // time since we last switched / epoch time
  unsigned long scale = now % 1000;
  uint8_t scale_8 = scale *255/1000;
  blend(prev_led, target_led, led, BUFFER_LENGTH, scale_8);
  FastLED.show();
  // 
  delay(30);
  count++;
}
