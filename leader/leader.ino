#include <FastLED.h>

#include <SPI.h>

#include <RadioHead.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

#include <ColorMap.h>
#include <support.h>
#include <constants.h>
#include <progmem.h>
#include <Painter.h>


// input is the cause of color change
uint8_t input = 1; // set to one so that in the first pass of the loop we dont reinitialize

RH_RF69 radio;
RHReliableDatagram sender(radio, FROM_ADDRESS);

// the array used to set the lights
CRGB led[ledBufferLength];
uint8_t rows[] = {4, 3, 2, 1, 0};
uint8_t cols[] = {0, 1, 2, 3, 4};

Painter painter(rows, cols, led);

// the array that gets sent over the radio
//const uint8_t outgoingPacketLength = 5;
uint8_t data[packetLength];


void initializeLeds()
{
    pinMode(LED_INDICATOR_PIN, OUTPUT);

    // Set uninitialised LEDs to a faint grey
    memset8(led, 60, ledBufferLength * sizeof(CRGB));

    FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(led, ledBufferLength).setCorrection(Typical8mmPixel);
    FastLED.show();
}

void initializeRadio()
{
  radio.init();
  radio.setFrequency(915.0);
  radio.setModemConfig(RH_RF69::GFSK_Rb19_2Fd38_4);
  sender.init();
}

void initializeRandom() {
  randomSeed(analogRead(0));
  random16_set_seed(random());
}

void patternChange(uint8_t* data) {
    data[0] = random8(nOffsets); // pattern/offset
    data[1] = random8(nColorMaps); // colormap
    data[2] = random8(); // starting color (argument for colormap)
    data[3] = random8(2);  // forwards or backwards
}

void ensureDelivery(uint8_t* data, uint8_t len, uint8_t to) {
  patternChange(data);
  painter.reset(data);
  while (true) {
    if (sender.sendtoWait(data, len, to)) {
      break;
    }
  }
}

void setup() {
  initializeLeds();
  initializeRadio();
  initializeRandom();
  ensureDelivery(data, packetLength, TO_ADDRESS);
}

void loop() {
  if (input == 0) {
    ensureDelivery(data, packetLength, TO_ADDRESS);
  }

  painter.paint(input);
  painter.wait();
  input += 1;
}

