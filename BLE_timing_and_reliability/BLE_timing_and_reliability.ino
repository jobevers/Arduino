// will attempt to read data
#include <SoftwareSerial.h>

#define LED_PIN LED_BUILTIN

SoftwareSerial bluetooth_serial(7, 8); // RX, TX  
// Connect HM10      Arduino Uno
//     Pin 1/TXD          Pin 7
//     Pin 2/RXD          Pin 8
volatile boolean bluetooth_serial_rate_detected = false;

long serial_rates[] = {
  9600, 57600, 115200,
  19200, 38400, 4800, 2400, 1200, 230400
};

bool discover_bluetooth_serial_rate() {
  for (int i = 0; i < (sizeof(serial_rates)/sizeof(long)); i++) {
    Serial.print(".");
    
    long rate = serial_rates[i];
    
    bluetooth_serial.begin(rate);
    bluetooth_serial.write("AT");
    bluetooth_serial.flush();
    if (bluetooth_serial.readString() == "OK") {
      Serial.println("\nBluetooth Serial Rate detected: " + String(rate));
      return true;
    } else {
      bluetooth_serial.end();
    }
  }

  Serial.println("\nBluetooth Serial Rate not detected.");
  
  return false;
}


void setup() {
  Serial.begin(9600); //2400
  //bluetooth_serial_rate_detected = 9600;
  bluetooth_serial.begin(9600);
  // AT+BAUD0 = 9600
  // AT+BAUD1 = 9600
  // AT+BAUD2 = 2400
  // AT+BAUD3 = 4800
  // AT+BAUD4 = 9600
  // AT+BAUD5 = 19200
  // AT+BAUD6 = 38400
  // AT+BAUD7 = 57600
  // AT+BAUD8 = 115200
  //bluetooth_serial_rate_detected = discover_bluetooth_serial_rate();
}

void loop() {
  //if(bluetooth_serial_rate_detected) {
    //int sensorValue = analogRead(A0);
    //Serial.println(sensorValue);

    if (bluetooth_serial.available()) {
      String s = "";
      uint8_t c;
      while((c = bluetooth_serial.read()) != 255) {
        Serial.println("Received: " + String(c));      
      }
      Serial.println("---");
    }
  
    if (Serial.available()) {
      String s = "";
      char c;
      while((c = Serial.read()) != -1) {
        s += c;
        delay(5);
      }
  
      Serial.println("Sent: " + s);
      
      bluetooth_serial.print(s);
    }
  //}
}
