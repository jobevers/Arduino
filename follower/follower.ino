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
uint8_t input = 1;

RH_RF69 radio;
RHReliableDatagram reciever(radio, TO_ADDRESS);

// the array used to set the lights
CRGB led[ledBufferLength];
uint8_t rows[] = {4, 3, 2, 1, 0};
uint8_t cols[] = {5, 6, 7, 8, 9};

Painter painter(rows, cols, led);

// the array that gets sent over the radio
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
  reciever.init();
}

void ensureReception() {
  while (true) {
    uint8_t incomingPacketLength = packetLength;
    if (reciever.recvfromAckTimeout(data, &incomingPacketLength, 1000, NULL, NULL, NULL, NULL)) {
      painter.reset(data);
      break;
    }
  }
}

void setup() {
  initializeLeds();
  initializeRadio();
  ensureReception();
}


void loop() {
  if (input == 0) {
    ensureReception();
  }
  
  painter.paint(input);
  painter.wait();
  input++;
}

