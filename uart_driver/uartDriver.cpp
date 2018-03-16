#include "uartDriver.hpp"
//#include "LPC17xx.h"
#include "lpc_isr.h"

LabUART::LabUART() {}
LabUART::~LabUART() {}

bool LabUART::init(Peripheral peripheral)	
{
  //uart 2 pins
  const uint8_t PCONP_UART2_PIN = 24;
  const uint8_t PCLKSEL_UART2_BIT = 16;
  const uint8_t TX2_BIT = 17;
  const uint8_t RX2_BIT = 19;

  //uart 3 pins
  const uint8_t PCONP_UART3_PIN = 25;
  const uint8_t PCLKSEL_UART3_BIT = 18;
  const uint8_t TX3_BIT = 25;
  const uint8_t RX3_BIT = 27; 
  
  const uint8_t DLAB_UART2_BIT = 7;
  const uint8_t RX_INTERRUPT_BIT = 0;
  
  
  bool return_val = false;
  if(peripheral != 0 && peripheral != 1){
    return_val = true; 
  }
  
  if(return_val == true){
    if(peripheral == 2){
      // Enable UART2 Peripheral
      LPC_SC->PCONP |= (1 << PCONP_UART2_PIN);
      
      // Clear and set clk for UART2. Clk/1
      LPC_SC->PCLKSEL1 &= ~(3 << PCLKSEL_UART2_BIT);
      LPC_SC->PCLKSEL1 |= (1 << PCLKSEL_UART2_BIT);
      
      // Set up TX (P2.8) and RX (P2.9)
      LPC_PINCON->PINSEL4 |= (1 << RX2_BIT);
      LPC_PINCON->PINSEL4 |= (1 << TX2_BIT);
      
      this->UART = LPC_UART2; 
    }
    if(peripheral == 3){
      // Enable UART2 Peripheral
      LPC_SC->PCONP |= (1 << PCONP_UART3_PIN);
      
      // Clear and set clk for UART2. Clk/1
      LPC_SC->PCLKSEL1 &= ~(3 << PCLKSEL_UART3_BIT);
      LPC_SC->PCLKSEL1 |= (1 << PCLKSEL_UART3_BIT);
      
      // Set up TX (P2.8) and RX (P2.9)
      LPC_PINCON->PINSEL4 |= (1 << RX3_BIT);
      LPC_PINCON->PINSEL4 |= (1 << TX3_BIT);
      
      this->UART = LPC_UART3; 
    }
  }
  
  // Set the UART to do 8 bit transactions
  this->UART->LCR |= (3 << 0);
  
  // Enable DLAB for UART2 to change baud rate
  this->UART->LCR |= (1 << DLAB_UART2_BIT);
  
  this->UART->DLL = (uint8_t) (312 & 0xFF);
  this->UART->DLM = (uint8_t) (312 >> 8);
  
  // Disable DLAB for UART2 to prevent baud rate change
  this->UART->LCR &= ~(1 << DLAB_UART2_BIT);
  
  // Enable interrupts on RX
  this->UART->IER |= (1 << RX_INTERRUPT_BIT);
}


void LabUART::transmit(char c)
{
  const uint8_t DONE_BIT = 6;
  // Send the character over UART2 interface
  this->UART->THR = c;
  
  // Loop until send is complete
  while(this->UART->LSR & (1 << DONE_BIT));	
}

char LabUART::receive(void)
{
  const uint8_t DONE_BIT = 0;
  char c;
  
  // Loop until all data is sent
  while(!(this->UART->LSR & (1 << DONE_BIT)));
  
  c = this->UART->RBR;
  return c;
}
