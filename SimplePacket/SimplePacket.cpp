#include "SimplePacket.h"

void SimplePacket::init(SoftwareSerialXT *serialt, uint8_t length){
  serial = serialt;
  buffer_index = 0;
  rcv_length = 0;
  found_header = false;

  // Add one to length to accomodate checksum during read
  rcv_buffer = (uint8_t*) malloc(length+1);
}

void SimplePacket::send(uint8_t *msg, uint8_t length){
  uint8_t checksum = length;
  serial->write(START_FLAG);
  serial->write(length);
  // Calculate simple XOR checksum and write out data
  for (char i = 0; i < length; i++){
    checksum ^= *(msg+i);
    serial->write(*(msg+i));
  }
  serial->write(checksum);
  serial->write(END_FLAG);
}

boolean SimplePacket::receive(uint8_t *msg, uint8_t length){

  uint8_t x;
  uint8_t checksum;

  if (serial->available()){
    x = serial->read();

    if (x == START_FLAG){
      found_header = true;
      buffer_index = 0;
      rcv_length = 0;
    }
    else if (x == END_FLAG){
      // Check if something was read (at least 1 byte and 
      // checksum) and if reported length was how much we 
      // read and if how much we read matches with the 
      // passed in msg length
      if (buffer_index >=2 && (buffer_index-1) == rcv_length && (buffer_index-1) == length){
        // Seems like we got everything, so calculate 
        // checksum on received packet
        checksum = rcv_length;
        for (char i = 0; i < rcv_length; i++)
          checksum ^= rcv_buffer[i];
        // Compare calculated checksum with received checksum
        if (checksum == rcv_buffer[buffer_index-1]){
          // Packet is good
          memcpy(msg, rcv_buffer, length);
          rcv_length = 0;
          buffer_index = 0;
          found_header = false;
          return true;
        }
      }
      // If we get here then packet is messed up, clear out everything
      rcv_length = 0;
      buffer_index = 0;
      found_header = false;
    }
    else if (found_header && rcv_length == 0){
      // Make sure msg size on both ends match
      if (x == length)
        rcv_length = x;
      else {
        found_header = false;
        buffer_index = 0;
      }
    }
    else if (found_header && rcv_length) {
      rcv_buffer[buffer_index++] = x;
    }

  }
  return false;
}
