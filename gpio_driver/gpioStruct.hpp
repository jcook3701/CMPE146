#ifndef LABGPIO_struct_H
#define LABGPIO_struct_H

#include "LPC17xx.h"

/** Void function pointer */
typedef void (*void_func_t)(void);

struct GPIO_Package{
  char *  function_name;
  bool *  globalVar; 
  uint8_t pin; 
  uint8_t port; 
};

#endif
