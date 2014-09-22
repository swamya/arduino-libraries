
/* This program uses a version of SoftwareSerial
   and SimplePacket to receive the number of times 
   the receiver should blink its light. While the 
   libary and the example use software serial, they 
   can easily be modified to support hardware serial 
   or any other communication method (e.g. I2C). 
*/
#include <SoftwareSerialXT.h>
#include <SimplePacket.h>

SoftwareSerialXT softSerial(2,3, false);
SimplePacket sp;

// The structs need to be the same on receiver and sender
struct PACKET_STRUCT {
  uint8_t blinks;
};

PACKET_STRUCT myData;

void setup() {
  softSerial.begin(9600);
  // Initialize the library and pass in serial pointer and data length
  sp.init(&softSerial, sizeof(myData));
  
  pinMode(13, OUTPUT);
}

void loop() { 
  
  if (sp.receive((byte *)&myData, sizeof(myData))){
     for (int i = 0; i < myData.blinks; i++){
        digitalWrite(13, HIGH);
        delay(100);
        digitalWrite(13, LOW);
        delay(100);
     } 
  }
  delay(500);
}
