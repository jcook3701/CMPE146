#include "LabGPIO_0.hpp"
#include <stdio.h>

LabGPIO_0::LabGPIO_0(uint8_t my_pin){
  this->pin = my_pin;
  //Driver is for port 0 so I don't see any need to set a port var...
}

void LabGPIO_0::setAsInput(void)          { LPC_GPIO0->FIODIR &= ~(1 << this->pin);                   }
void LabGPIO_0::setAsOutput(void)         { LPC_GPIO0->FIODIR |= (1 << this->pin);                    }
void LabGPIO_0::setDirection(bool output) { output-true? setAsOutput() : setAsInput();                }
void LabGPIO_0::setHigh(void)             { LPC_GPIO0->FIOPIN |= (1 << this->pin);                    }
void LabGPIO_0::setLow(void)              { LPC_GPIO0->FIOPIN &= ~(1 << this->pin);                   }
void LabGPIO_0::set(bool high)            { high-true? setHigh() : setLow();                          }
bool LabGPIO_0::getLevel(void)            { return LPC_GPIO0->FIOPIN & (1 << this->pin)? true: false; }
