#include "LabGPIO_1.hpp"
#include <stdio.h>

LabGPIO_1::LabGPIO_1(uint8_t my_pin){
  this->pin = my_pin;
  //Driver is for port 0 so I don't see any need to set a port var...
}

void LabGPIO_1::setAsInput(void)          { LPC_GPIO1->FIODIR &= ~(1 << this->pin);                  }
void LabGPIO_1::setAsOutput(void)         { LPC_GPIO1->FIODIR |= (1 << this->pin);                   }
void LabGPIO_1::setDirection(bool output) { output-true? setAsOutput() : setAsInput();               }
void LabGPIO_1::setHigh(void)             { LPC_GPIO1->FIOPIN |= (1 << this->pin);                    }
void LabGPIO_1::setLow(void)              { LPC_GPIO1->FIOPIN &= ~(1 << this->pin);                   }
void LabGPIO_1::set(bool high)            { (high)?setHigh():setLow();                               }
bool LabGPIO_1::getLevel(void)            { return LPC_GPIO1->FIOPIN & (1 << this->pin)? true: false;}
