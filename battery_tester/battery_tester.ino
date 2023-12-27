//
// Run a battery powering a string of LEDS
// and monitor the voltage drop on the battery.
// 
// Output the values on the serial line
//

#include <FastLED.h>

const int N_LEDS = 100;
// the array used to set the lights
CRGB led[N_LEDS];


const int LED_INDICATOR_PIN = 13;
const long vcc = 4890; // measured using volt meter

void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  // Set uninitialised LEDs to full white
  memset8(led, 200, N_LEDS * sizeof(CRGB));

  Serial.begin(9600);      // open the serial port at 9600 bps:
  Serial.print("Time\tVoltage\n");

  const int output_pin = 6;
  FastLED.addLeds<WS2811, output_pin, RGB>(led, N_LEDS).setCorrection(Typical8mmPixel);
  FastLED.show(); 
}


void loop() {
  digitalWrite(LED_INDICATOR_PIN, HIGH);
  Serial.print(millis());
  Serial.print("\t");
  double volt = (analogRead(0) / 1023.0) * vcc;
  Serial.print(volt);
  Serial.print("\n");
  delay(100);
  digitalWrite(LED_INDICATOR_PIN, LOW);
  delay(9900);
  FastLED.show();
}


long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
     ADMUX = _BV(MUX5) | _BV(MUX0) ;
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
