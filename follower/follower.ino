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
RHReliableDatagram reciever(radio, TO_ADDRESS);

// the array used to set the lights
CRGB led[BUFFER_LENGTH];
uint8_t incomingPacketLength = 5;
// the array that gets sent over the radio
uint8_t data[5];
bool firstReception = true;

ColorMap* cm;
uint8_t currentOffset = 0;
uint8_t rows[] = {4, 3, 2, 1, 0};
uint8_t cols[] = {5, 6, 7, 8, 9};

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
  reciever.init();
}

void setOffset(uint8_t offsetIdx) {
  makePattern(offset, rows, 5, cols, 5, allOffsets[offsetIdx]); 
}

void setup() {
  initializeLeds();
  initializeRadio();
  setOffset(currentOffset);
}

void loop() {
  if (reciever.recvfromAckTimeout(data, &incomingPacketLength, 200, NULL, NULL, NULL, NULL)) {
    // The processing between here and below should be the same between both
    // the leader and the follower
    if (data[0] == 0 || firstReception) {
      setOffset(data[1]);
      currentOffset = data[1];
      delete cm;
      cm = createColorMap(data[2], data[3]);
      firstReception = false;
    }
    for (int i=0; i<BUFFER_LENGTH; i++) {
      uint8_t color = data[4] ? offset[i] + data[0] : offset[i] - data[0];
      led[i] = cm->color(color);
    } 
    FastLED.show();
  }
}

