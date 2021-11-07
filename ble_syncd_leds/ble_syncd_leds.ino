/* Read in serial data from a BLE device and display the LEDS
 * 
 */
#include <FastLED.h>

// this is how many pixels are on the front
// The back will just be a duplicate
#define N 3
// Total number of pixels, should be 2*N
#define BUFFER_LENGTH 6
#define LED_INDICATOR_PIN LED_BUILTIN
// Wire the LED data line to this pin
#define LED_DATA_PIN 4

// The buffer that contains the LEDs to show
CRGB led[BUFFER_LENGTH];
// We interpolate / blend from prev -> target
//
// Consider these "key" frames. And the other frames are
// interpolations between them.
CRGB prev_led[N];
CRGB target_led[N];
// new data from the HM10 gets stored in next
CRGB next_led[N];

unsigned long prev_time;
unsigned long target_time;
// when to switch key frames
unsigned long next_time;
// The difference between our time and the server time
// Not sure yet how to calculate this, or how often.
uint16_t time_offset = 0;
// set to true when all of the data for the next key frame
// has been recieved
bool have_next = false;

// how to copy the LEDS from the front to the back
// need to actually fill this out.  Just have some
// dummy values in there right now.
int ledMap[] = {
  25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
  35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
  45, 46, 47, 48, 49
};



void setup() {
  // Have to worry about interrupts: https://github.com/FastLED/FastLED/wiki/Interrupt-problems
  // Can't really go faster than this.  It takes 30*50 = 1500us = 1.5ms to send the pixel data.
  // It takes 1.04 ms to recieve a byte. We have a buffer of 1 byte, as long as the interrupt
  // is paused less then 2.08ms we'll be ok. So, at 9600 we shouldn't lose data.
  //
  // And, we're also okay to read in the full 20 bytes = 20.8ms, so we shouldn't miss a frame.
  Serial.begin(9600);

  pinMode(LED_INDICATOR_PIN, OUTPUT);

  // Set uninitialised LEDs to a faint grey
  memset8(led, 255, BUFFER_LENGTH * sizeof(CRGB));

  // TODO: should ws2811 be changed to ws2812b?
  FastLED.addLeds<WS2811, LED_DATA_PIN, GRB>(led, BUFFER_LENGTH).setCorrection(Typical8mmPixel);
  FastLED.show();
}

void loop() {
  if (Serial.available()) {
    // 20 bytes how much is sent over BLE in each update.
    // So it just seems easier to read the updates in chunks of
    // 20 bytes.
    uint8_t data[20];
    // Will take 20.8 ms to ready 20 bytes (at 9600 baud)
    Serial.readBytes(data, 20);
    // Currently only two message types:
    // 0 -> The even pixels
    // 1 -> The odd pixels
    uint8_t msg = data[0];
    if (msg == 0) {
      next_time = (data[1] * 255 + data[2]) * 10 + time_offset;
    } else {
      have_next = true;
    }
    for (int i = 0; 2 * i + msg < N; i++) {
      next_led[2 * i + msg] = unpack(data[3 + i]);
    }
  }

  // TODO: what if we haven't recieved all of the next key frame yet?!
  //       I can't just wait, but I also have bad info.
  //       Can try to send back some sort of confirmation?
  unsigned long now = millis();
  if (have_next && (now >= next_time)) {
    for (int i = 0; i < N; i++) {
      prev_led[i] = target_led[i];
      target_led[i] = next_led[i];
    }
    prev_time = target_time;
    target_time = next_time;
    have_next = false;
  }

  EVERY_N_MILLISECONDS(30) {
    interpolate();
    copyAndShow();
  }
}

// we don't have enough bandwidth to send a full 3 bytes
// per pixel so we're going with 16 different hues, 4 saturations and 4 values.
// There might be more useful ways of sending 256 colors.
CHSV unpack(uint8_t datum) {
  // 16 different hues, evenly spaced:
  // 0,16,32,48,...,224,240
  // First 4 bits:    11110000
  uint8_t hue = (datum & 0xF0);
  // This returns 63, 127, 191, 255
  // The next 2 bits: 00001100
  uint8_t sat = ((datum & 0x0C) << 4) | 0x3F;
  // This returns 63, 127, 191, 255
  // The last 2 bits: 00000011
  uint8_t val =  ((datum & 0x03) << 6) | 0x3F;
  return CHSV(hue, sat, val);
}

void interpolate() {
  uint8_t scale = getScale();
  // Does this blend in RGB space (probably) or HSV space?
  // and do I care?
  // If prev_led and target_led were CHSV it would blend in HSV space.
  // TODO: convert all of the LED arrays to CHSV! 
  blend(prev_led, target_led, led, N, scale);
}

uint8_t getScale() {
  unsigned long now = millis();
  // Is there a faster, beter, smarter way to get the scale?
  // Note that the 255 being first is imporant.  Otherwise
  // we'll get integer division -> 0 * 255 and that is wrong.
  return 255 * (now - prev_time) / (target_time - prev_time);
}

// Copy the front to the back and show
void copyAndShow() {
  for (int i = 0; i < N; i++) {
    led[ledMap[i]] = led[i];
  }
  FastLED.show();
}
