#include <SPI.h>

#include <RadioHead.h>
#include <RH_RF69.h>


RH_RF69 radio;
int led = 9;
uint8_t rxBuffer[1] = {0};
uint8_t incomingPacketLength = 1;


void setup() {
  radio.init();
  radio.setFrequency(915.0);
  radio.setModemConfig(RH_RF69::GFSK_Rb19_2Fd38_4);
 
  pinMode(led, OUTPUT);
}


void loop() {
   digitalWrite(led, HIGH);
  // put your main code here, to run repeatedly:
  if (radio.available()) {
    radio.recv(rxBuffer, &incomingPacketLength);
    for(int i=0; i<rxBuffer[0]; i++) {
      digitalWrite(led, LOW);
      delay(100);
      digitalWrite(led, HIGH);
      delay(100);
    } 
  }
}
