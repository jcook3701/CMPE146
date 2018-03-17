#ifndef PRODUCER_CONSUMER_struct_H
#define PRODUCER_CONSUMER_struct_H

#include "LPC17xx.h"

enum orientation_t
{
  invalid = 0,
  up      = 1,
  down    = 2,
  left    = 3,
  right   = 4
};

struct producer_consumer_package{
  bool *  globalVar; 

};


#endif
