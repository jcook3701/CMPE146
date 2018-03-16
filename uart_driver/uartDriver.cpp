#include "uartDriver.hpp"
#include "LPC17xx.h"
#include "lpc_isr.h"

LabUART::LabUART() {}
LabUART::~LabUART() {}

void LabUART::init(void)	
{
	const uint8_t PCONP_UART2_PIN = 24;
	const uint8_t PCLKSEL_UART2_BIT = 16;
	const uint8_t TX_BIT = 17;
	const uint8_t RX_BIT = 19;
	const uint8_t DLAB_UART2_BIT = 7;
	const uint8_t RX_INTERRUPT_BIT = 0;

	// Enable UART2 Peripheral
	LPC_SC->PCONP |= (1 << PCONP_UART2_PIN);

	// Clear and set clk for UART2. Clk/1
	LPC_SC->PCLKSEL1 &= ~(3 << PCLKSEL_UART2_BIT);
	LPC_SC->PCLKSEL1 |= (1 << PCLKSEL_UART2_BIT);

	// Set up TX (P2.8) and RX (P2.9)
	LPC_PINCON->PINSEL4 |= (1 << RX_BIT);
	LPC_PINCON->PINSEL4 |= (1 << TX_BIT);

	// Set the UART to do 8 bit transactions
	LPC_UART2->LCR |= (3 << 0);
	
	// Enable DLAB for UART2 to change baud rate
	LPC_UART2->LCR |= (1 << DLAB_UART2_BIT);

	LPC_UART2->DLL = (uint8_t) (312 & 0xFF);
	LPC_UART2->DLM = (uint8_t) (312 >> 8);
	
	// Disable DLAB for UART2 to prevent baud rate change
	LPC_UART2->LCR &= ~(1 << DLAB_UART2_BIT);

	// Enable interrupts on RX
	LPC_UART2->IER |= (1 << RX_INTERRUPT_BIT);
}


void LabUART::transmit(char c)
{
	const uint8_t DONE_BIT = 6;
	// Send the character over UART2 interface
	LPC_UART2->THR = c;

	// Loop until send is complete
	while(LPC_UART2->LSR & (1 << DONE_BIT));	
}

char LabUART::receive(void)
{
	const uint8_t DONE_BIT = 0;
	char c;
	
	// Loop until all data is sent
	while(!(LPC_UART2->LSR & (1 << DONE_BIT)));

	c = LPC_UART2->RBR;
	return c;
}
