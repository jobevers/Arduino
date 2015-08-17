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


static int input = 0;
ColorMap cm;
RH_RF69 radio;
RHReliableDatagram sender(radio, FROM_ADDRESS);
CRGB led[BUFFER_LENGTH];
uint8_t offset[BUFFER_LENGTH];
uint8_t data[1];


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

void setup() {
  initializeLeds();
  initializeRadio();
}

void loop() {
  uint8_t color = input*12; 
  data[0] = color;
  if (sender.sendtoWait(data, 1, TO_ADDRESS)) {
    FastLED.showColor(CHSV(color, 255, 255));
    delay(50);
    input += 1;
  } else {
    FastLED.showColor(CHSV(0, 0, 255));
    delay(100);
    FastLED.showColor(CHSV(0, 0, 0));
    delay(100);
  }
}

