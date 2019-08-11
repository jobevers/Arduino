/* Read in serial data from a BLE device and display the LEDS

    This is version of the program that only:
    - has a fixed time between frames

*/
#include <FastLED.h>
#include <SoftwareSerial.h>

#define DEBUG 0
// This allows us to hookup a second arduino
// to see what data is coming in from the BLE
// (But, slows down the whole process as we now
//  need to wait for the second serial action to finish)
// Load the serial_to_serial sketch on the other arduino
// and hookup pins 2 & 3 on each board (cross them!)
#if DEBUG == 1
const byte rxPin = 2;
const byte txPin = 3;
SoftwareSerial mySerial(rxPin, txPin);
#endif

// this is how many pixels are on the front
#define N 20
// And we have no back, so this is the same
#define BUFFER_LENGTH 20
#define LED_INDICATOR_PIN LED_BUILTIN
// Wire the LED data line to this pin
#define LED_DATA_PIN 4

// The buffer that contains the LEDs to show
CRGB led[BUFFER_LENGTH];
// for interpolation, need an HSV array
// which gets copied into LED right before show()
CHSV hsv[N];
// We interpolate / blend from prev -> target
//
// Consider these "key" frames. And the other frames are
// interpolations between them.
CHSV prev_led[N];
CHSV target_led[N];
// new data from the HM10 gets stored in next
CHSV next_led[N];
// How long between key frames
const int KEY_FRAME_DURATION = 250;

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

void setup() {
  // Have to worry about interrupts: https://github.com/FastLED/FastLED/wiki/Interrupt-problems
  // Can't really go faster than this.  It takes 30*50 = 1500us = 1.5ms to send the pixel data.
  // It takes 1.04 ms to recieve a byte. We have a buffer of 1 byte, as long as the interrupt
  // is paused less then 2.08ms we'll be ok. So, at 9600 we shouldn't lose data.
  //
  // And, we're also okay to read in the full 20 bytes = 20.8ms, so we shouldn't miss a frame.
  Serial.begin(9600);
#if DEBUG == 1
  mySerial.begin(9600);
#endif

  pinMode(LED_INDICATOR_PIN, OUTPUT);

  // Set uninitialised LEDs to a faint grey
  // just so that we know that they are on
  memset8(led, 2, BUFFER_LENGTH * sizeof(CRGB));
  memset8(prev_led, 2, BUFFER_LENGTH * sizeof(CRGB));
  memset8(target_led, 32, BUFFER_LENGTH * sizeof(CRGB));
  memset8(next_led, 128, BUFFER_LENGTH * sizeof(CRGB));

  // My test string of LEDs:
  // FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(led, BUFFER_LENGTH).setCorrection(Typical8mmPixel);
  // The actual jackets
  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(led, BUFFER_LENGTH).setCorrection(Typical8mmPixel);
  FastLED.show();
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (Serial.available()) {
    digitalWrite(LED_BUILTIN, HIGH);
    // 20 bytes is how much is sent over BLE in each update.
    // So it just seems easier to read the updates in chunks of
    // 20 bytes.
    uint8_t data[20];
    // Will take 20.8 ms to ready 20 bytes (at 9600 baud)
    Serial.readBytes(data, 20);
    uint8_t msg = data[0];
    // Maybe send back a timestamp here
    Serial.write(msg);
#if DEBUG == 1
    mySerial.write(data, 20);
#endif
    // For msg = 0, this reads in the even pixels (0, 2, 4..)
    // and for msg = 1, this reads in the odd pixels (1, 3, 5..)
    int offset = 1; // Need to account for the first msg byte
    for (int i = 0; 2 * i + msg < N; i++) {
      next_led[2 * i + msg] = unpack(data[i + offset]);
    }
#if DEBUG == 1
    //mySerial.write(next_led[0].hue);
#endif
    have_next = true;
  }

  // TODO: what if we haven't recieved all of the next key frame yet?!
  //       I can't just wait, but I also have bad info.
  unsigned long now = millis();
  if (now >= target_time) {
    if (have_next) {
      for (int i = 0; i < N; i++) {
        prev_led[i] = target_led[i];
        target_led[i] = next_led[i];
      }
    } else {
      // In the situation we don't have the updated frame
      // setting prev = target just means that the pixels
      // stay constant for 250ms. Which isn't great
      // but at least it isn't a flicker.
      for (int i = 0; i < N; i++) {
        prev_led[i] = target_led[i];
      }
    }
    prev_time = target_time;
    target_time = next_time;
    // Have a fixed 'framerate' of 250 ms
    next_time += KEY_FRAME_DURATION;
    have_next = false;
    digitalWrite(LED_BUILTIN, LOW);
  }

  // I have no idea how this work (try reading the code, good luck).
  // but this should happen every frame.
  EVERY_N_MILLISECONDS(30) {
    interpolate();
    FastLED.show();
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
  // we blend into HSV so that this uses HSV for the interpolation
  blend(prev_led, target_led, hsv, N, scale);
  // and then copy the results into the LED array.
  // which is where FASTLed converts it into RGB for us.
  for (int i = 0; i < N; i++) {
    led[i] = hsv[i];
  }
#if DEBUG == 1
  //  mySerial.write(prev_led[0].hue);
  //  mySerial.write(hsv[0].hue);
  //  mySerial.write(target_led[0].hue);
#endif
}

uint8_t getScale() {
  unsigned long now = millis();
  // Is there a faster, beter, smarter way to get the scale?
  // Note that the 255 being first is imporant.  Otherwise
  // we'll get integer division -> 0 * 255 and that is wrong.
  if (now > target_time) {
#if DEBUG == 1
    //mySerial.println("!");
#endif
    return 255;
  }
  return 255 * (now - prev_time) / (target_time - prev_time);
}
