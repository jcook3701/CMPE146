#include "FreeRTOS.h"
#include "tasks.hpp"
#include "printf_lib.h"
#include "uart0_min.h"
#include "stdio.h"
#include "LPC17xx.h"
#include "lpc_isr.h"

#include "uartDriver.hpp"

LabUART UART3_mp3;

QueueHandle_t charQueue;

void print_lcd_test(void* pvparams)
{
  char i = 0;
  int run_number = 1; 
  
  while(1)
    {
      UART3_mp3.transmit('J');
      UART3_mp3.transmit('a');
      UART3_mp3.transmit('r');
      UART3_mp3.transmit('e');
      UART3_mp3.transmit('d');
      UART3_mp3.transmit(' ');
      UART3_mp3.transmit('C');
      UART3_mp3.transmit('o');
      UART3_mp3.transmit('o');
      UART3_mp3.transmit('k');
      u0_dbg_printf("Test Serial Port 3\n");      
      vTaskDelay(2000);
    }
}

/*
uint8_t make_result(char* buffer)
{
  const char operand_1 = buffer[0];
  const char operand_2 = buffer[1];
  const char operation = buffer[2];
  const uint8_t operand_1_val = operand_1 - '0';
  const uint8_t operand_2_val = operand_2 - '0';
  uint8_t answer = 0;
  
  switch(operation)
    {
    case '+':
      answer = operand_1_val + operand_2_val;
      break;
    case '-':
      answer = operand_1_val - operand_2_val;
      break;
    case '*':
      answer = operand_1_val * operand_2_val;
      break;
    default:
      break;
    }
  
  return answer;
}

void read_char(void* pvparams)
{
  char c;
  int i = 0;
  char input[3] = { 0 };
  char buf[2] = { 0 };
  uint8_t answer;
  
  
  while(1)
    {
      if(xQueueReceive(charQueue, &c, 1000))
	{
	  printf("Got %c\n", c);
	  input[i] = c;
	  ++i;
	  
	  if(i == 3)
	    {
	      answer = make_result(input);							
	      UART2.transmit((char) answer);			
	      printf("Sending back: %c %c %c = %d\n", input[0], input[2], input[1], answer);
	      i = 0;
	    }
	}
    }
}

void print_answer(void* pvparams)
{
  char c;	
  while(1)
    {
      if(xQueueReceive(charQueue, &c, 1000))
	{
	  printf("Sender received: %d\n", (uint8_t) c);
	}
    }
}

void rx_int_handler(void)
{
  char c;
  c = UART2.receive();
  xQueueSend(charQueue, &c, 1000);
}

*/

int main(int argc, char const *argv[])
{
  const unsigned int pclk = sys_get_cpu_clock();
  u0_dbg_printf("clk speed: %d!\n", pclk);

  Peripheral device;
  device = UART3; 
  UART3_mp3.init(pclk, device, 9600);
  //UART3_mp3.set_lcd_baudrate();
  //isr_register(UART3_IRQn, rx_int_handler);
  NVIC_EnableIRQ(UART3_IRQn);
  
  charQueue = xQueueCreate(100, sizeof(char));

  scheduler_add_task(new terminalTask(PRIORITY_HIGH));
  //xTaskCreate(print_char, "print_char", 512, (void*) 1, 1, NULL );
  //xTaskCreate(print_answer, "answer", 512, (void*) 1, 1, NULL );
  xTaskCreate(print_lcd_test, "print_lcd_test", 512, (void*) 1, 1, NULL);
  
  scheduler_start();
  
  return false;
}

