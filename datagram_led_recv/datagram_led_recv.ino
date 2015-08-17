#include <FastLED.h>

#include <SPI.h>

#include <RadioHead.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

#include <ColorMap.h>


#define BUFFER_LENGTH 50 // >100LEDs unsupported
#define NUM_LEDS 50
#define LED_DATA_PIN 5
#define LED_DATA_PIN_SECOND_HALF 7
#define INPUT_PIN 4
#define LED_INDICATOR_PIN 9 // Hardwired on the board
#define FROM_ADDRESS 11
#define TO_ADDRESS 12


class Pattern {
public:
    Pattern(){};
    void offsets(int, uint8_t[]);
};

void Pattern::offsets(int input, uint8_t offset[]) {
  int idx = 0;
  for (int fb=0; fb<2; fb++) { 
    for (int row=5; row<10; row++) {
      for (int col=0; col<5; col++) {
        if (!fb) {
          offset[idx++] = row;
        } else {
          offset[idx++] = 14 - row;
        }
      }
    }
  }
}

RH_RF69 radio;
RHReliableDatagram reciever(radio, TO_ADDRESS);
CRGB led[BUFFER_LENGTH];
uint8_t offset[BUFFER_LENGTH];
uint8_t rxBuffer[1] = {0};
uint8_t incomingPacketLength = 1;
//static uint8_t input = 0;
ColorMap cm;
Pattern pattern;

void initializeLeds()
{
    pinMode(LED_INDICATOR_PIN, OUTPUT);

    // Set uninitialised LEDs to a faint grey
    memset8(led, 255, BUFFER_LENGTH * sizeof(CRGB));

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
}

void loop() {
  if (reciever.recvfromAckTimeout(rxBuffer, &incomingPacketLength, 200, NULL, NULL, NULL, NULL)) {
    FastLED.showColor(CHSV(rxBuffer[0], 255, 255));
  } else {
//    FastLED.showColor(CHSV(0, 0, 255));
//    delay(100);
//    FastLED.showColor(CHSV(0, 0, 0));
//    delay(100);
  }
}
