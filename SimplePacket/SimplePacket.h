/*
 * Simple Packet Library for use with Software Serial
 * 
 */

#ifndef SimplePacket_h
#define SimplePacket_h

#include "Arduino.h"
#include <SoftwareSerialXT.h>

#include <stdint.h>

class SimplePacket {
 public:
  void init(SoftwareSerialXT *, uint8_t); 
  void send(uint8_t *, uint8_t);
  boolean receive(uint8_t *, uint8_t);
  
 private:
  SoftwareSerialXT *serial;
  const uint8_t START_FLAG = 0xFE;
  const uint8_t END_FLAG = 0x7F; 
  
  boolean found_header;
  uint8_t buffer_index;
  uint8_t rcv_length;
  uint8_t *rcv_buffer;  
};

#endif
