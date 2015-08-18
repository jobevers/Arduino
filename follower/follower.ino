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


#define BUFFER_LENGTH 50 // >100LEDs unsupported
#define NUM_LEDS 50
#define LED_DATA_PIN 5
#define LED_DATA_PIN_SECOND_HALF 7
#define INPUT_PIN 4
#define LED_INDICATOR_PIN 9 // Hardwired on the board
#define FROM_ADDRESS 11
#define TO_ADDRESS 12


// input is the cause of color change
uint8_t input = 1;

// the array used to make patterns
uint8_t offset[BUFFER_LENGTH];

RH_RF69 radio;
RHReliableDatagram reciever(radio, TO_ADDRESS);

// the array used to set the lights
CRGB led[BUFFER_LENGTH];
// the array that gets sent over the radio
uint8_t data[5];
bool firstReception = true;

ColorMap* cm;
uint8_t currentOffset = 0;
uint8_t rows[] = {4, 3, 2, 1, 0};
uint8_t cols[] = {5, 6, 7, 8, 9};

Painter painter(rows, cols, led);

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



void setup() {
  initializeLeds();
  initializeRadio();
  while (true) {
    uint8_t incomingPacketLength = packetLength;
    if (reciever.recvfromAckTimeout(data, &incomingPacketLength, 1000, NULL, NULL, NULL, NULL)) {
      painter.reset(data);
      break;
    }
  }
}

void initializeColorMapAndOffset(uint8_t* data) {
  delete cm;
  cm = createColorMap(data[2], data[3]);
  //setOffset(data[1]);
}


void loop() {
  if (input == 0) {
    while (true) {
      uint8_t incomingPacketLength = packetLength;
      if (reciever.recvfromAckTimeout(data, &incomingPacketLength, 1000, NULL, NULL, NULL, NULL)) {
        painter.reset(data);
        break;
      }
    }
  }
  
  painter.paint(input);
  painter.wait();
  input++;
}

