#ifndef LABGPIO_struct_H
#define LABGPIO_struct_H

#include "LPC17xx.h"
#include "gpioDriver.hpp"
#include "spiDriver.hpp"

/** Void function pointer */
typedef void (*void_func_t)(void);

struct GPIO_Package{
  char *  function_name;
  bool *  globalVar; 
  uint8_t pin; 
  uint8_t port; 
};

struct GPIO_SPI_Package{
  bool *  globalVar; 
  LabSPI *spi_object;
  LabGPIO *gpio_object; 
};


#endif
