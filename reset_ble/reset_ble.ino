/* A work in progress. The idea here is that I could just run a sketch
 *  and reset the settings on the BLE devices.
 * 
 * 
 */
String NAME="FRANCINE5";

// list of hc-10 AT commands
// http://fab.cba.mit.edu/classes/863.15/doc/tutorials/programming/bluetooth/bluetooth40_en.pdf

// If you haven't configured your device before use this
#define BLUETOOTH_SPEED 9600 //This is the default baudrate that HC-10 uses

#include <SoftwareSerial.h>


// Swap RX/TX connections on bluetooth chip
//   Pin 7 RX --> Bluetooth TX
//   Pin 8 TX --> Voltage Divider -> Bluetooth RX
SoftwareSerial mySerial(7, 8); // RX, TX

/*
  The posible baudrates are:
    0---------9600
    1---------19200
    2---------38400
    3---------57600
    4---------115200
    5---------4800
    6---------2400
    7---------1200
    8---------230400
    Default: 0(9600)
*/

/* In setup we run a bunch of queries against the HM10.
 * And then in the look you can send AT commands to the HM10
 * via the serial console.
 */

void setup() {
  //pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  //digitalWrite(9, HIGH);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("Starting config");
  mySerial.begin(BLUETOOTH_SPEED);
  delay(1000);

  // Should respond with OK
  Serial.println("AT");
  mySerial.print("AT");
  waitForResponse();

  Serial.println("AT+ADDR?");
  mySerial.print("AT+ADDR?");
  waitForResponse();

  Serial.println("AT+ADVI?");
  mySerial.print("AT+ADVI?");
  waitForResponse();

  Serial.println("AT+ANCS?");
  mySerial.print("AT+ANCS?");
  waitForResponse();

  Serial.println("AT+ALLO?");
  mySerial.print("AT+ALLO?");
  waitForResponse();

  Serial.println("AT+BATC?");
  mySerial.print("AT+BATC?");
  waitForResponse();

  Serial.println("AT+BATT?");
  mySerial.print("AT+BATT?");
  waitForResponse();

  Serial.println("AT+BIT7?");
  mySerial.print("AT+BIT7?");
  waitForResponse();

  Serial.println("AT+BAUD?");
  mySerial.print("AT+BAUD?");
  waitForResponse();

  Serial.println("AT+CHAR?");
  mySerial.print("AT+CHAR?");
  waitForResponse();

  Serial.println("AT+COL??");
  mySerial.print("AT+COL??");
  waitForResponse();

  Serial.println("AT+CYC??");
  mySerial.print("AT+CYC??");
  waitForResponse();

  Serial.println("AT+DISC?");
  mySerial.print("AT+DISC?");
  waitForResponse();

  Serial.println("AT+FILT?");
  mySerial.print("AT+FILT?");
  waitForResponse();

  Serial.println("AT+FIOW?");
  mySerial.print("AT+FIOW?");
  waitForResponse();

  Serial.println("AT+HELP?");
  mySerial.print("AT+HELP?");
  waitForResponse();

  Serial.println("AT+IMME?");
  mySerial.print("AT+IMME?");
  waitForResponse();

  Serial.println("AT+IBEA?");
  mySerial.print("AT+IBEA?");
  waitForResponse();

  Serial.println("AT+IBE0?");
  mySerial.print("AT+IBE0?");
  waitForResponse();

  Serial.println("AT+MARJ?");
  mySerial.print("AT+MARJ?");
  waitForResponse();

  Serial.println("AT+NOTI?");
  mySerial.print("AT+NOTI?");
  waitForResponse();

  Serial.println("AT+POWE?");
  mySerial.print("AT+POWE?");
  waitForResponse();

  Serial.println("AT+PWRM?");
  mySerial.print("AT+PWRM?");
  waitForResponse();

  Serial.println("AT+ROLE?");
  mySerial.print("AT+ROLE?");
  waitForResponse();

  Serial.println("AT+RSSI?");
  mySerial.print("AT+RSSI?");
  waitForResponse();

  Serial.println("AT+RADD?");
  mySerial.print("AT+RADD?");
  waitForResponse();

  Serial.println("AT+RAT??");
  mySerial.print("AT+RAT??");
  waitForResponse();

  Serial.println("AT+STOP?");
  mySerial.print("AT+STOP?");
  waitForResponse();

  Serial.println("AT+SAVE?");
  mySerial.print("AT+SAVE?");
  waitForResponse();

  Serial.println("AT+SHOW?");
  mySerial.print("AT+SHOW?");
  waitForResponse();

  Serial.println("AT+UUID?");
  mySerial.print("AT+UUID?");
  waitForResponse();

  Serial.println("AT+UART?");
  mySerial.print("AT+UART?");
  waitForResponse();

  Serial.println("AT+VERR?");
  mySerial.print("AT+VERR?");
  waitForResponse();

  Serial.println("AT+VERS?");
  mySerial.print("AT+VERS?");
  waitForResponse();

  Serial.println("AT+PASS?");
  mySerial.print("AT+PASS?");
  waitForResponse();

  Serial.println("AT+NAME?");
  mySerial.print("AT+NAME?");
  waitForResponse();

  Serial.println("Done with Query!");

  Serial.println("Setting properties");
  Serial.println("AT+NAME" + NAME);
  mySerial.print("AT+NAME" + NAME);
  waitForResponse();
  Serial.println("AT+NAME?");
  mySerial.print("AT+NAME?");
  waitForResponse();

  Serial.println("AT+PASS328228");
  mySerial.print("AT+PASS328228");
  waitForResponse();
  Serial.println("AT+PASS?");
  mySerial.print("AT+PASS?");
  waitForResponse();

  // 3 = Require passcode and pair
  Serial.println("AT+TYPE3");
  mySerial.print("AT+TYPE3");
  waitForResponse();
  Serial.println("AT+TYPE?");
  mySerial.print("AT+TYPE?");
  waitForResponse();

  // Disable connection notifications
  // If set to 1,  OK+CONN or OK+LOST
  // are sent when connected or disconnected
  Serial.println("AT+NOTI0");
  mySerial.print("AT+NOTI0");
  waitForResponse();
  Serial.println("AT+NOTI?");
  mySerial.print("AT+NOTI?");
  waitForResponse();

  Serial.println();
  Serial.println("Enter your own AT commands:");

  
  
}

void waitForResponse() {
  delay(1000);
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  Serial.write("\n");
}


void loop() {
  if (mySerial.available()) {
    String s = "";
    char c;
    while ((c = mySerial.read()) != -1) {
      s += c;
      delay(5);
    }
    Serial.println("Received: " + s);
  }

  if (Serial.available()) {
    String s = "";
    char c;
    while ((c = Serial.read()) != -1) {
      s += c;
      delay(5);
    }

    Serial.println("Sent: " + s);

    mySerial.print(s);
  }
}
