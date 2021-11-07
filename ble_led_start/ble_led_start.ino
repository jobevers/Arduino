#include <SoftwareSerial.h>
#include <FastLED.h>

#define BUFFER_LENGTH 50
#define LED_INDICATOR_PIN LED_BUILTIN
#define LED_DATA_PIN 4

SoftwareSerial bluetooth_serial(7, 8); // RX, TX
// Connect HM10      Arduino Uno
//     Pin 1/TXD          Pin 7
//     Pin 2/RXD          Pin 8
volatile boolean bluetooth_serial_rate_detected = false;
CRGB led[BUFFER_LENGTH];


void initializeLeds()
{
  pinMode(LED_INDICATOR_PIN, OUTPUT);

  // Set uninitialised LEDs to a faint grey
  memset8(led, 255, BUFFER_LENGTH * sizeof(CRGB));

  FastLED.addLeds<WS2811, LED_DATA_PIN, GRB>(led, BUFFER_LENGTH).setCorrection(Typical8mmPixel);
  FastLED.show();

}

int idx = 0;

void setup() {
  initializeLeds();

  Serial.begin(9600);
  bluetooth_serial_rate_detected = 9600;
  bluetooth_serial.begin(9600);
  //bluetooth_serial_rate_detected = discover_bluetooth_serial_rate();
}

uint8_t bytes_expected = 0xFF;
// 0 = even pixels
// 1 = odd pixels
uint8_t odd_or_even = 0xFF;
uint8_t bytes_recieved = 0xFF;
int count = 0;
 
void loop() {
    uint8_t c;
    if (bluetooth_serial.available()) {
      c = bluetooth_serial.read();
      Serial.print(c);
      Serial.print('-');
      if (bytes_expected == 0xFF) {
        bytes_expected = c;
        bytes_recieved = 0;
        return;
      }
      if (odd_or_even == 0xFF) {
        odd_or_even = c;
        return;
      }
      int idx = bytes_recieved * 2 + odd_or_even;
      led[idx].setHue(c);
      bytes_recieved += 1;
      if (bytes_recieved == bytes_expected) {
        Serial.println();
        Serial.println(count++);
        FastLED.show();
        bytes_expected = 0xFF;
        bytes_recieved = 0xFF;
        odd_or_even = 0xFF;
      }
    }
}
