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
   unsigned int mPeripheralClock; 
public:
  LabUART();
  ~LabUART();
  
  // TODO: Fill in methods for init(), transmit(), receive() etc.
  //  bool init(Peripheral peripheral);
  bool init(unsigned int pclk, Peripheral peripheral, unsigned int baudRate);
  void setBaudRate(unsigned int baudRate);
  void set_lcd_baudrate();
  void transmit(char c);
  char receive(void);
  
  // Optional: For the adventurous types, you may inherit from "CharDev" class to get a lot of funcionality for free
};

#endif
