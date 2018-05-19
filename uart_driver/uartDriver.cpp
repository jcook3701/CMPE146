#include "uartDriver.hpp"
//#include "LPC17xx.h"
#include "lpc_isr.h"

LabUART::LabUART() {}
LabUART::~LabUART() {}

void LabUART::setBaudRate(unsigned int baudRate)
{
    /* Adding 0.5 to perform rounding correctly since we do not want
     * 1.9 to round down to 1, but we want it to round-up to 2.
     */

    this->UART->LCR = (1 << 7); // Enable DLAB to access DLM, DLL, and IER
    {
        uint16_t bd = (mPeripheralClock / (16 * baudRate)) + 0.5;
        this->UART->DLM = (bd >> 8);
        this->UART->DLL = (bd >> 0);
    }
    this->UART->LCR = 3; // Disable DLAB and set 8bit per char
}

void LabUART::set_lcd_baudrate(){
  const uint8_t DLAB_UART_BIT = 7;
  
  // Enable DLAB for UART to change baud rate
  //this->UART->LCR |= ~(1 << DLAB_UART_BIT);
  this->UART->LCR = ( 1 << 7); 
  // DLM
  this->UART->DLM = (uint8_t) (0 << 0);
  // DLL
  this->UART->DLL = (uint8_t) (125 << 0);
  // DIVADDVAL
  this->UART->FDR = ( 3 << 0);
  // MULVAL
  this->UART->FDR = ( 2 << 4); 
  // Disable DLAB for UART2 to prevent baud rate change
  this->UART->LCR = 3; //&= ~(1 << DLAB_UART_BIT);
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


bool LabUART::init(unsigned int pclk, Peripheral peripheral, unsigned int baudRate)	
{
  
  this->mPeripheralClock = pclk;

  //uart 0 pins
  const uint8_t PCONP_UART0_PIN = 3;

  //uart 1 pins
  const uint8_t PCONP_UART1_PIN = 4;
  
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
      this->UART = LPC_UART2;

      // Enable UART2 Peripheral
      LPC_SC->PCONP |= (1 << PCONP_UART2_PIN);
      
      // Clear and set clk for UART2. Clk/1
      LPC_SC->PCLKSEL1 &= ~(3 << PCLKSEL_UART2_BIT);
      LPC_SC->PCLKSEL1 |= (1 << PCLKSEL_UART2_BIT);
      
      // Set up TX (P2.8) and RX (P2.9)
      LPC_PINCON->PINSEL4 |= (1 << RX2_BIT);
      LPC_PINCON->PINSEL4 |= (1 << TX2_BIT);
      
    }
    if(peripheral == 3){
      this->UART = LPC_UART3;
      
      // Enable UART2 Peripheral
      LPC_SC->PCONP |= (1 << PCONP_UART3_PIN);
      
      // Clear and set clk for UART2. Clk/1
      LPC_SC->PCLKSEL1 &= ~(3 << PCLKSEL_UART3_BIT);
      LPC_SC->PCLKSEL1 |= (1 << PCLKSEL_UART3_BIT);
      
      // Set up TX (P2.8) and RX (P2.9)
      LPC_PINCON->PINSEL4 |= (1 << RX3_BIT);
      LPC_PINCON->PINSEL4 |= (1 << TX3_BIT);
    }
  }


  // Set the UART to do 8 bit transactions
  //this->UART->LCR |= (3 << 0);

  setBaudRate(baudRate);
  /*
  // Enable DLAB for UART to change baud rate
  this->UART->LCR |= (1 << DLAB_UART2_BIT);
  
  this->UART->DLL = (uint8_t) (312 & 0xFF);
  this->UART->DLM = (uint8_t) (312 >> 8);
  
  // Disable DLAB for UART to prevent baud rate change
  this->UART->LCR &= ~(1 << DLAB_UART2_BIT);
  */
  
  // Enable interrupts on RX
  this->UART->IER |= (1 << RX_INTERRUPT_BIT);
}

