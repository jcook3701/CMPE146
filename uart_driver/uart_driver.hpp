#ifndef LABUART_H
#define LABUART_H

#include "LPC17xx.h"

class LabUART
{
private:
public:
  LabUART();
  ~LabUART();
  
  // TODO: Fill in methods for init(), transmit(), receive() etc.
  bool init();
  uint8_t transmit(uint8_t send);
  uint8_t receive(); 
  
  // Optional: For the adventurous types, you may inherit from "CharDev" class to get a lot of funcionality for free
};

#endif
