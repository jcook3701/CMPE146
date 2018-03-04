#include "eintDriver.hpp"
#include "printf_lib.h"

//---------------------------Singleton--------------------------------//
// Global static pointer used to ensure a single instance of the class.
//LabGPIOInterrupts* LabGPIOInterrupts::instance = 0;
/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/
/*
LabGPIOInterrupts* LabGPIOInterrupts::Instance(){
  if(!instance){
    instance = new LabGPIOInterrupts(); 
  }
  return instance; 
}
*/  
//--------------------------------------------------------------------//
LabGPIOInterrupts::LabGPIOInterrupts(){
  for(int i = 0; i < data_width; i++){
    p0_falling[i] = 0;
    p0_rising[i]  = 0;
    p2_falling[i] = 0;
    p2_rising[i]  = 0; 
  }
}

LabGPIOInterrupts::~LabGPIOInterrupts(){

}

void LabGPIOInterrupts::init(void (*pin_isr)(void)){
  // Register your ISR using lpc_isr.h
  isr_register(EINT3_IRQn, pin_isr);
  // Enable interrupt for the EINT3
  NVIC_EnableIRQ(EINT3_IRQn);
}

bool LabGPIOInterrupts::attachInterruptHandler(uint8_t port, uint32_t pin, void (*pin_isr)(void), InterruptCondition_E condition){
  this->my_pin = pin; 
  if(port == 0){
    if(rising_edge == condition){
      p0_rising[pin] = pin_isr;
      LPC_GPIOINT->IO0IntEnR |=(1<<pin);
      this->IntClr = &LPC_GPIOINT->IO0IntClr; 
      return true;
    }
    if(falling_edge == condition){
      p0_falling[pin] = pin_isr;
      LPC_GPIOINT->IO0IntEnF |=(1<<pin);
      this->IntClr = &LPC_GPIOINT->IO0IntClr;
      return true;
    }
    if(both_edges == condition){
      p0_rising[pin] = pin_isr;
      p0_falling[pin] = pin_isr;
      LPC_GPIOINT->IO0IntEnR |=(1<<pin); 
      LPC_GPIOINT->IO0IntEnF |=(1<<pin);
      this->IntClr = &LPC_GPIOINT->IO0IntClr;
      return true; 
    }
  }
  else if(port == 2){
    if(rising_edge == condition){
      p2_rising[pin] = pin_isr;
      LPC_GPIOINT->IO2IntEnR |=(1<<pin);
      this->IntClr = &LPC_GPIOINT->IO2IntClr;
      return true; 
    }
    if(falling_edge == condition){
      p2_falling[pin] = pin_isr;
      LPC_GPIOINT->IO2IntEnF |=(1<<pin);
      this->IntClr = &LPC_GPIOINT->IO2IntClr;
      return true; 
    }
    if(both_edges == condition){
      p2_rising[pin] = pin_isr;
      p2_falling[pin] = pin_isr;
      LPC_GPIOINT->IO2IntEnR |=(1<<pin);
      LPC_GPIOINT->IO2IntEnF |=(1<<pin);
      this->IntClr = &LPC_GPIOINT->IO2IntClr;
      return true; 
    }
  }
  return false; 
}

void LabGPIOInterrupts::clearInterruptFlag(void){
  *(this->IntClr) |=(1<<this->my_pin); 
}

void LabGPIOInterrupts::handle_interrupt(void){
  uint32_t p0_rising_inter = LPC_GPIOINT->IO0IntStatR;
  uint32_t p2_rising_inter = LPC_GPIOINT->IO2IntStatR;
  uint32_t p0_falling_inter = LPC_GPIOINT->IO0IntStatF;
  uint32_t p2_falling_inter = LPC_GPIOINT->IO2IntStatF;
  
  for(int i = 0; i < data_width; i++){
    if(p0_rising_inter & (1 << i)){
      if(p0_rising[i] != 0){
	p0_rising[i](); 
      }
    }
    if(p0_falling_inter & (1 << i)){
      if(p0_falling[i] != 0){
	p0_falling[i](); 
      }
    }
    if(p2_rising_inter & (1 << i)){
      if(p2_rising[i] != 0){
	p2_rising[i](); 
      }
    }
    if(p2_falling_inter & (1 << i)){
      if(p2_falling[i] != 0){
	p2_falling[i](); 
      }
    }
  }
}


