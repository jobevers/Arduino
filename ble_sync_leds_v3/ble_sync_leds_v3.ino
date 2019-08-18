/* Read in serial data from a BLE device and display the LEDS

*/
#include <FastLED.h>
#include <SoftwareSerial.h>

// Wire the LED data line to this pin
#define LED_DATA_PIN 4

// Have multiple debug levels because print a lot
// slows down everything! So, debug 1 will print just a minimal amount
// to hint at the state
// debug 2 prints the actual messages recieved and occasionally the state
// debug 3 prints the actual messages recieved and verbosely the state
#define DEBUG 1
// This allows us to hookup a second arduino
// to see what data is coming in from the BLE
// (But, slows down the whole process as we now
//  need to wait for the second serial action to finish)
// Load the serial_to_serial sketch on the other arduino
// and hookup pins 2 & 3 on each board (cross them!)
// Arduino2 pin 2 (RX) -> Arduino 1 pin 3 (TX)
// Arduino2 pin 3 (TX) -> Arduino 1 pin 2 (RX)
#if DEBUG == 1
const byte rxPin = 2;
const byte txPin = 3;
SoftwareSerial mySerial(rxPin, txPin);
#endif

// How many bytes get sent over BLE
const int payloadSize = 20;
// this is how many pixels are on the front
const int N = 25;
// And with the back, this is the total length
const int BUFFER_LENGTH = 50;
// The size of the ring buffer we are using for
// the "key" frames.
const uint8_t RING_SIZE = 6;

// The buffer that contains the LEDs to show
CRGB led[BUFFER_LENGTH];
// for interpolation, need an HSV array
// which gets copied into LED right before show()
CHSV hsv[N];

// Consider these "key" frames. And the other frames are
// interpolations between them.
CHSV frames[RING_SIZE][N];
// saves when each frame should be shown
uint16_t frameNum[N];
// which frame in the ring is the "present" one
uint8_t presentFrameIdx = 0;
// which frame in the ring is the "target" one
uint8_t targetFrameIdx = 1;
// where in the ring new data should go
uint8_t incomingFrameIdx = 0;
// track how many frames we have saved in the ring
uint8_t fullFrameCount = 0;
// a counter for the frames
// this wraps at 65535, which is every 36 minutes.
// TODO: do a reset before this happens!
uint16_t frameN = 0;
// each frame takes two messages to fill and so this helps
// keep track of whether we've recieved both of those
uint8_t nextMessage = 0;

// on startup we want to fill up the ring before doing anything
// so don't display anything until
bool loading = true;

// How much to increase the frame count by. This can be set by Android in a message
// so that a slow device can be sped up.
uint8_t frameInc = 1;
// We want the frame ring buffer to always be near 5 frames
// and so I'm going to slow frameN as the ring buffer gets empty
// and need to keep a counter so that I can skip updates occasionally
uint8_t frameIncCounter = 0;

// how far along we are from present -> target frames.
// 0 = at present frame
// 255 = at target frame
fract8 scale = 1;
// how much to increase scale on each framestep
fract8 deltaScale = 0;

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

  pinMode(LED_BUILTIN, OUTPUT);

  // Set uninitialised LEDs to a faint grey
  // just so that we know that they are on
  memset8(led, 4, BUFFER_LENGTH * sizeof(CRGB));
  memset8(frames, 4, RING_SIZE * N * sizeof(CHSV));

#if DEBUG == 1
  // My test string of LEDs:
  FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(led, BUFFER_LENGTH).setCorrection(Typical8mmPixel);
#else
  // The actual jackets
  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(led, BUFFER_LENGTH).setCorrection(Typical8mmPixel);
#endif
  FastLED.show();
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // want the frameInc setting from the Android to only be applied once
  // otherwise it seems scary to apply it many times until the next message
  // comes in.
  frameInc = 1;
  readIncomingData();

  // I have no idea how this work (try reading the code, good luck).
  // but this should happen every frame.
  // 30fps = 1000 / 30 = 33;
  EVERY_N_MILLISECONDS(33) {
    playFrame();
  }
}

// a little helper function to increase the idx values in the ring buffer
static uint8_t ringInc(uint8_t idx) {
  return (idx + 1) % RING_SIZE;
}

void readIncomingData() {
  if (!Serial.available()) {
    return;
  }
  // blink when we get new data
  digitalWrite(LED_BUILTIN, HIGH);
  // 20 bytes is how much is sent over BLE in each update.
  // So it just seems easier to read the updates in chunks of
  // 20 bytes.
  uint8_t data[payloadSize];
  // Will take 20.8 ms to ready 20 bytes (at 9600 baud)
  Serial.readBytes(data, payloadSize);
#if DEBUG == 1
  mySerial.println("M");
# elif DEBUG == 2
  for (int i = 0; i < payloadSize; i++) {
    mySerial.print(data[i], HEX);
    mySerial.print(" ");
  }
  mySerial.println();
#endif
  // The first four bits specify the message type
  uint8_t msg = (data[0] & 0xF0) >> 4;
  if (msg == 0 || msg == 1) {
    if (msg != nextMessage) {
#if DEBUG == 1
      mySerial.println("MISMATCH");
#endif
      // Something got messed up.  We should get out of here.
      // reset nextMessage = 0 so that we can start fresh.
      nextMessage = 0;
      if (msg == 0) {
        // Don't send msg1 because we'll have to ignore it.
        sendRejectResponse();
      } else {
        // reject isn't necessary here because we want msg0 next
        sendResponse();
      }
      digitalWrite(LED_BUILTIN, LOW);
      return;
    }
    if (fullFrameCount == RING_SIZE) {
#if DEBUG == 1
      mySerial.println("FULL");
#endif
      // Our ring buffer is already full, so we need to get out of here
      nextMessage = 0;
      sendRejectResponse();
      digitalWrite(LED_BUILTIN, LOW);
      return;
    }
    // Can add between 0 and 15 frames the next time
    // we increment frames
    frameInc = data[0] & 0x0F;
    CHSV* frame = frames[incomingFrameIdx];
    // this is when this frame should be shown
    uint16_t myFrameNum = data[1] << 8 | data[2];
    frameNum[incomingFrameIdx] = myFrameNum;
    // For msg = 0, this reads in the even pixels (0, 2, 4..)
    // and for msg = 1, this reads in the odd pixels (1, 3, 5..)
    int offset = 3; // Need to account for the first msg byte and the
    for (int i = 0; 2 * i + msg < N; i++) {
      frame[2 * i + msg] = unpack(data[i + offset]);
    }
    if (msg == 1) {
      if (fullFrameCount == 1) {
        // If the frame rate on the two devices starts to get off
        // we could end up in the perverse situation in which
        // our frameN is larger than the myFrameNum in the message
        // and so... it seems the only thing to do would be to knock
        // the frameN back down.
        if (frameN > myFrameNum) {
          frameN = myFrameNum;
        } else {
          // Since there was only one frame left in the buffer, there
          // was no target frame and were were just showing the present
          // frame without any interpolation.
          // Now that we have a second frame, we'll be interpolating
          // and we need to set the scale here.
          setScale();
        }
      }
      nextMessage = 0;
      fullFrameCount++;
      incomingFrameIdx = ringInc(incomingFrameIdx);
#if DEBUG >= 2
      mySerial.println("ADDED");
      printDebug();
#endif
    } else {
      nextMessage = 1;
    }
    sendResponse();
    digitalWrite(LED_BUILTIN, LOW);
    return;
  }
}

// we don't have enough bandwidth to send a full 3 bytes
// per pixel so we're going with 32 different hues, 4 saturations and 2 values.
// I put together a little jupyter notebook looking at some different
// options and this seemed to be a pretty reasonable one.
//
// Not that I'm still leaving some bytes unused.  We actually have 34 bytes
// for the 25 pixels, which is 10 bits per pixel, which could be
// 128 hues, 4 saturations and 2 values but for a lot more book-keeping.
CHSV unpack(uint8_t datum) {
  // 32 different hues, evenly spaced:
  // 0,8,16,24,...,232,240,248
  // First 5 bits:    11111000
  uint8_t hue = (datum & 0xF8);
  // This returns 63, 127, 191, 255
  // The next 2 bits: 00000110
  uint8_t sat = ((datum & 0x06) << 5) | 0x3F;  // 3F -> 0011 1111; so we're filling the last six bits with 1s
  // This returns 127, 255
  // The last 1 bits: 00000001
  uint8_t val =  ((datum & 0x01) << 7) | 0x7F; // 7F -> 0111 1111; so we're filling the last seven bits with 1s
  return CHSV(hue, sat, val);
}


void sendResponse() {
  uint8_t response[] = {
    0x00, (uint8_t)((frameN & 0xFF00) >> 8), (uint8_t)(frameN & 0x00FF), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,                              0x00,                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  Serial.write(response, payloadSize);
}

void sendRejectResponse() {
  // Send 1 as the second byte to tell android NOT to send us the
  // second message in this frame. Probably because our ring buffer
  // is full and we'd just have to throw out the data.
  // TODO: actually implement this ignore on the Android side
  uint8_t response[] = {
    0x00, (uint8_t)((frameN & 0xFF00) >> 8), (uint8_t)(frameN & 0x00FF),  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,                              0x00,                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };
  Serial.write(response, payloadSize);
}


void setScale() {
  // scale is a value between 0-255 that determines how much blend to do.
  // when frameN = frameNum[presentFrameIdx] then scale = 0
  // when frameN = frameNum[targetFrameIdx]  then scale = 255
  deltaScale = 256 / (frameNum[targetFrameIdx] - frameNum[presentFrameIdx]);
  scale = deltaScale * (frameN - frameNum[presentFrameIdx]);
}


void playFrame() {
  if (loading) {
    // Don't fully load the ring buffer.
    if (fullFrameCount < RING_SIZE - 1) {
      // we're still loading
      return;
    }
#if DEBUG == 1
    mySerial.println("DONE LOADING");
#endif
    loading = false;
    frameN = frameNum[presentFrameIdx];
    setScale();
  }
  setFrameInc();
  frameN += frameInc;

#if DEBUG == 2
  mySerial.print("N: ");
  mySerial.println(frameN);
#elif DEBUG >= 3
  printDebug()
#endif
  // if we reset the Scale in the next block than we don't need to
  // apply the deltaScale as the scale will already be calculated correctly
  boolean resetScale = false;
  while (fullFrameCount >= 2) {
    // Comparing frame numbers (frameN < frameNum[targetFrameIdx]) is dangerous
    // because they overflow.  But that only happens at 65536 so we just
    // need to make sure to reset before then!
    if (frameNum[presentFrameIdx] <= frameN && frameN < frameNum[targetFrameIdx]) {
      break;
    }
    resetScale = true;
    presentFrameIdx = ringInc(presentFrameIdx);
    targetFrameIdx = ringInc(targetFrameIdx);
    fullFrameCount--;
#if DEBUG == 1
    mySerial.println("SWITCHED FRAME");
    printDebug();
#endif
    setScale();
  }
  if (!resetScale) {
    scale += deltaScale * frameInc;
  }
  setLed();
  copyAndShow();
}

void setFrameInc() {
  // Try to speed up / slow down our frame count to adjust to what we're getting
  frameIncCounter += 1;
  if (fullFrameCount < 4) {
    // SLOW DOWN
    // If fullFrameCount = 2 then we skip every other frame so we're at 15.15 fps
    // If fullFrameCount = 3 then we skip every third frame so we're at 20.2  fps
    if ((frameIncCounter % fullFrameCount) == 0) {
      frameInc = 0;
    }
  } else if (fullFrameCount == 5) {
    // SPEED UP
    // If fullFrameCount = 5 then we double every  frame so we're at 60 fps
    frameInc = 2;
  } else if (fullFrameCount == 6) {
    // If fullFrameCount = 6 then we triple every frame so we're at 90 fps
    frameInc = 3;
  }
}

void printDebug() {
  mySerial.print("N: ");
  mySerial.print(frameN);
  mySerial.print(" ffc: ");
  mySerial.print(fullFrameCount);
  mySerial.print(" p: ");
  mySerial.print(presentFrameIdx);
  mySerial.print(" t: ");
  mySerial.print(targetFrameIdx);
  mySerial.print(" i: ");
  mySerial.print(incomingFrameIdx);
  mySerial.print(" ");
  // Print when we're supposed to switch frames
  for (int i = 0; i < fullFrameCount; i++) {
    mySerial.print(frameNum[(presentFrameIdx + i) % RING_SIZE]);
    mySerial.print(",");
  }
  mySerial.println();
}

void setLed() {
  if (fullFrameCount >= 2) {
#if DEBUG >= 3
    // interpolation seems to be working so I'm moving that to level 2 debug
    mySerial.print("INTERP ");
    mySerial.println(scale);
#endif
    interpolate();
  } else {
#if DEBUG == 1
    mySerial.println("CURRENT");
#endif
    // With only one frame we have no choice but to
    // just show the present one
    for (int i = 0; i < N; i++) {
      led[i] = frames[presentFrameIdx][i];
    }
  }
}


void interpolate() {
  // we blend into HSV so that this uses HSV for the interpolation
  blend(frames[presentFrameIdx], frames[targetFrameIdx], hsv, N, scale);
  // and then copy the results into the LED array.
  // which is where FASTLed converts it into RGB for us.
  for (int i = 0; i < N; i++) {
    led[i] = hsv[i];
  }
}


void copyAndShow() {
  for (int i = N; i < BUFFER_LENGTH; i++) {
    // This mapping -> the back is a reflection of the front
    led[i] = led[BUFFER_LENGTH - (i + 1)];
  }
  FastLED.show();
}
