 /*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x70


SoftwareSerial mySerial(10, 11); // RX, TX

long value=123456;
char buf[100];
String incoming;
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 Serial.println("Serial ready");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);

    while (!mySerial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("MySerial ready");
  Wire.begin(SLAVE_ADDRESS);
 Wire.onRequest(sendData);
  
}

void loop() { // run over and over
  if (mySerial.available()) {
    incoming = mySerial.readString();
    Serial.print("mySerial received: ");
    Serial.println(incoming);
  }
       
       mySerial.print(value);
      delay(10);
        Serial.println("...loop...");
     value=value+1;
}

void sendData(){
 
//  itoa(value,buf,10);
   value=value+1;
  ltoa(value,buf,10);
  Wire.write(buf);
  
  Serial.println("===on request sendData()  Wire.write ...itoa[value,buf,10]");
  
}




