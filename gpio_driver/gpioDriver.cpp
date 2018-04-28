#include "gpioDriver.hpp"
#include <stdio.h>

LabGPIO::LabGPIO(uint8_t my_port, uint8_t my_pin){
  this->port = my_port; 
  this->pin = my_pin;
  
  if(this->port == 0){
    this->FIODIR = &LPC_GPIO0->FIODIR;
    this->FIOPIN = &LPC_GPIO0->FIOPIN; 
  }
  else if(this->port == 1){
    this->FIODIR = &LPC_GPIO1->FIODIR;
    this->FIOPIN = &LPC_GPIO1->FIOPIN; 
  }
  else if(this->port == 2){
    this->FIODIR = &LPC_GPIO2->FIODIR;
    this->FIOPIN = &LPC_GPIO2->FIOPIN; 
  }
  else if(this->port == 4){
    this->FIODIR = &LPC_GPIO4->FIODIR;
    this->FIOPIN = &LPC_GPIO4->FIOPIN; 
  }
}

void LabGPIO::setAsInput(void)          { *(this->FIODIR) &= ~(1 << this->pin);                   }
void LabGPIO::setAsOutput(void)         { *(this->FIODIR) |= (1 << this->pin);                    }
void LabGPIO::setDirection(bool output) { output-true? setAsOutput() : setAsInput();              }
void LabGPIO::setHigh(void)             { *(this->FIOPIN) |= (1 << this->pin);                    }
void LabGPIO::setLow(void)              { *(this->FIOPIN) &= ~(1 << this->pin);                   }
void LabGPIO::set(bool high)            { high-true? setHigh() : setLow();                        }
bool LabGPIO::getLevel(void)            { return *(this->FIOPIN) & (1 << this->pin)? true: false; }
LabGPIO::~LabGPIO() {};
