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

void loop() {
  if (mySerial.available()) {
    char c;
    c = mySerial.read();
    Serial.print(c);
  };
}
