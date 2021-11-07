// Read in software serial and write out to uart serial
// This is useful for debugging.  Can hook up
// one uno to another one and see what is coming through
// over bluetooth

#include <SoftwareSerial.h>

const byte rxPin = 2;
const byte txPin = 3;
SoftwareSerial mySerial(rxPin, txPin);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("OK. LETS GO");
}

int idx = 0;
uint8_t data[20];

void loop() {
  while (mySerial.available()) {
    uint8_t d;
    d = mySerial.read();
    data[idx] = d;
    idx++;
    if (idx >= 20) {
      for (int i = 0; i<20; i++) {
        Serial.print(data[i], HEX);
        Serial.print(" ");
      }
      Serial.print("\n");
      idx = 0;
    }
//    char c;
//    c = mySerial.read();
//    Serial.println(c);
  }
}
