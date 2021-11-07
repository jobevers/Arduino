#include <SPI.h>

#include <RadioHead.h>
#include <RH_RF69.h>


RH_RF69 radio;
uint8_t data[1] = {1};
int led = 9;

void setup() {
  radio.init();
  radio.setFrequency(915.0);
  radio.setModemConfig(RH_RF69::GFSK_Rb19_2Fd38_4);

  randomSeed(analogRead(0));
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led, HIGH);
  delay(1000);
  data[0] = random(1, 6);
  radio.send(data, 1);
  radio.waitPacketSent();
  for(int i=0; i<data[0]; i++) {
    digitalWrite(led, LOW);
    delay(100);
    digitalWrite(led, HIGH);
    delay(100);
  }
}
