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

// the setup routine runs once when you press reset:
void setup() {
  initializeLeds(); 
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(LED_INDICATOR_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);               // wait for a second
  digitalWrite(LED_INDICATOR_PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(200);               // wait for a second
}
