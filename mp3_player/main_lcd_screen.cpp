#include "FreeRTOS.h"
#include "tasks.hpp"
#include "printf_lib.h"
#include "uart0_min.h"
#include "stdio.h"
#include "LPC17xx.h"
#include "lpc_isr.h"

#include "uart3.hpp"
//#include "uartDriver.hpp"

//LabUART 
//UartDev *UART3_mp3;
Uart3 *u3;

QueueHandle_t charQueue;

void print_lcd_test(void* pvparams)
{
  char i = 0;
  int run_number = 1; 
  
  while(1)
    {
      u3->putChar('J');
      u3->putChar('a');
      u3->putChar('r');
      u3->putChar('e');
      u3->putChar('d');
      u3->putChar(' ');
      u3->putChar('C');
      u3->putChar('o');
      u3->putChar('o');
      u3->putChar('k');
	
      /*
      UART3_mp3.;
      UART3_mp3.transmit('a');
      UART3_mp3.transmit('r');
      UART3_mp3->transmit('e');
      UART3_mp3->transmit('d');
      UART3_mp3->transmit(' ');
      UART3_mp3->transmit('C');
      UART3_mp3->transmit('o');
      UART3_mp3->transmit('o');
      UART3_mp3->transmit('k');
      */
      u0_dbg_printf("Test Serial Port 3\n");      
      vTaskDelay(2000);
    }
}

int main(int argc, char const *argv[])
{

  const unsigned int wifi_rx_size = 0;
  const unsigned int wifi_tx_size = 100; 
  const unsigned int pclk = sys_get_cpu_clock();
  u0_dbg_printf("clk speed: %d!\n", pclk);

  
  *u3 = Uart3::getInstance();
  u3->init(9600, wifi_rx_size , wifi_tx_size);

  
  //UART3_mp3.init(pclk, device, 9600);
  //UART3_mp3.set_lcd_baudrate();
  //isr_register(UART3_IRQn, rx_int_handler);
  //NVIC_EnableIRQ(UART3_IRQn);
  
  charQueue = xQueueCreate(100, sizeof(char));

  scheduler_add_task(new terminalTask(PRIORITY_HIGH));
  //xTaskCreate(print_char, "print_char", 512, (void*) 1, 1, NULL );
  //xTaskCreate(print_answer, "answer", 512, (void*) 1, 1, NULL );
  xTaskCreate(print_lcd_test, "print_lcd_test", 512, (void*) 1, 1, NULL);
  
  scheduler_start();
  
  return false;
}

