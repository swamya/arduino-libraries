
/* This program uses a version of SoftwareSerial and
   SimplePacket to transmit the number of times the 
   receiver should blink its light. While the libary 
   and the example use software serial, they can easily 
   be modified to support hardware serial or any other 
   communication methods (e.g. I2C). 
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
int i = 0;

void setup() {
  softSerial.begin(9600);
  // Initialize the library and pass in serial pointer and data length
  sp.init(&softSerial, sizeof(myData));

}

void loop() { 
  myData.blinks = ++i;
  // Send packet 
  sp.send((byte *)&myData, sizeof(myData));
  if (i == 5) 
    i = 0;
  delay(500);
}
