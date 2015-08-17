#include <FastLED.h>

#include <SPI.h>

#include <RadioHead.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

#include <ColorMap.h>
#include <support.h>
#include <constants.h>
#include <progmem.h>

#define BUFFER_LENGTH 50 // >100LEDs unsupported
#define NUM_LEDS 50
#define LED_DATA_PIN 5
#define LED_DATA_PIN_SECOND_HALF 7
#define INPUT_PIN 4
#define LED_INDICATOR_PIN 9 // Hardwired on the board
#define FROM_ADDRESS 11
#define TO_ADDRESS 12


// input is the cause of color change
uint8_t input = 0;

// the array used to make patterns
uint8_t offset[BUFFER_LENGTH];

RH_RF69 radio;
RHReliableDatagram sender(radio, FROM_ADDRESS);

// the array used to set the lights
CRGB led[BUFFER_LENGTH];
uint8_t rows[] = {4, 3, 2, 1, 0};
uint8_t cols[] = {0, 1, 2, 3, 4};

ColorMap* cm;
uint8_t currentOffset;

// the array that gets sent over the radio
const uint8_t outgoingPacketLength = 5;
uint8_t data[outgoingPacketLength];


void initializeLeds()
{
    pinMode(LED_INDICATOR_PIN, OUTPUT);

    // Set uninitialised LEDs to a faint grey
    memset8(led, 60, BUFFER_LENGTH * sizeof(CRGB));

    FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(led, BUFFER_LENGTH).setCorrection(Typical8mmPixel);
    FastLED.show();
}

void initializeRadio()
{
  radio.init();
  radio.setFrequency(915.0);
  radio.setModemConfig(RH_RF69::GFSK_Rb19_2Fd38_4);
  sender.init();
}

void setOffset(uint8_t offsetIdx) {
  makePattern(offset, rows, 5, cols, 5, allOffsets[offsetIdx]); 
}

void setup() {
  initializeLeds();
  initializeRadio();
}

void loop() {
  if (input == 0) {
    delete cm;
    uint8_t colorMapIdx = random8(nColorMaps);
    uint8_t startingColor = random8();
    cm = createColorMap(colorMapIdx, startingColor);
    currentOffset = random8(nOffsets);
    setOffset(currentOffset);
    data[1] = currentOffset;
    data[2] = colorMapIdx;
    data[3] = startingColor;
    data[4] = random8(2);
  }

  data[0] = input;
  if (sender.sendtoWait(data, outgoingPacketLength, TO_ADDRESS)) {
    // The processing between here and below should be the same between both
    // the leader and the follower
    for (int i=0; i<BUFFER_LENGTH; i++) {
      uint8_t color = data[4] ? offset[i] + data[0] : offset[i] - data[0];
      led[i] = cm->color(color);
    } 
    FastLED.show();
    // End similarity

    delay(cm->delay);
    input += 1;
  } 
}

