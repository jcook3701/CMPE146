#ifndef LABUART_H
#define LABUART_H

#include "LPC17xx.h"
#include "sys_config.h"  // sys_get_cpu_clock()

enum Peripheral
  {
    UART2 = 2,
    UART3 = 3
  };

class LabUART
{
private:
  LPC_UART_TypeDef* UART;
public:
    LabUART();
    ~LabUART();
  
    // TODO: Fill in methods for init(), transmit(), receive() etc.
	bool init(Peripheral peripheral);
	void transmit(char c);
	char receive(void);
  
    // Optional: For the adventurous types, you may inherit from "CharDev" class to get a lot of funcionality for free
};

#endif
