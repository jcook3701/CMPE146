#include "FreeRTOS.h"
#include "tasks.hpp"
#include "uart0_min.h"
#include "stdio.h"
#include "LPC17xx.h"
#include "lpc_isr.h"

#include "io.hpp"
#include "uartDriver.hpp"


LabUART UART_PORT;

QueueHandle_t charQueue;

void print_char(void* pvparams)
{
  char i = 0;
  
  while(1)
    {	
      UART_PORT.transmit('5');
      UART_PORT.transmit('4');
      UART_PORT.transmit('+');
      
      UART_PORT.transmit('7');
      UART_PORT.transmit('6');
      UART_PORT.transmit('-');
      
      UART_PORT.transmit('9');
      UART_PORT.transmit('9');
      UART_PORT.transmit('*');
      vTaskDelay(2000);
    }
}

void print_with_buttons(void * pvparams)
  {
    int period = 0; 
    int count = 0;
    char ALU[3] = {'+', '-', '*'};
    int  alu_count = 0; 
    while(1)
      {
	if(SW.getSwitch(1))
	  {
	    count++; 
	  }
	if(SW.getSwitch(2) && count > 0 )
	  {
	    count--; 
	  }
	if(SW.getSwitch(3))
	  {
	    alu_count++;
	    alu_count %= 3;
	  }
	if(SW.getSwitch(4))
	  {
	    if(period != 2)
	      {
		UART_PORT.transmit(count + '0');
		period++;
	      }
	    else
	      {
		UART_PORT.transmit(ALU[alu_count]);
		period = 0;
	      }
	  }
	LD.setNumber(count);
	vTaskDelay(200);
      }
  }

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
	      UART_PORT.transmit((char) answer);			
	      printf("Sending back: %c %c %c = %d\n", input[0], input[2], input[1], answer);
	      LD.setNumber(answer); 
	      i = 0;
	    }
	}
    }
}

void print_answer(void* pvparams)
{
  static char c;	
  while(1)
    {
      
      if(xQueueReceive(charQueue, &c, 1000))
	{
	  printf("Sender received: %d\n", (uint8_t) c);
	}
      LD.setNumber(c); 
    }
}

void rx_int_handler(void)
{
  char c;
  c = UART_PORT.receive();
  xQueueSend(charQueue, &c, 1000);
}

int main(int argc, char const *argv[])
{
  LD.init();
  LD.clear();

  SW.init(); 

  UART_PORT.init(Peripheral::UART2);
  
  isr_register(UART2_IRQn, rx_int_handler);
  NVIC_EnableIRQ(UART2_IRQn);

  charQueue = xQueueCreate(100, sizeof(char));
  
  scheduler_add_task(new terminalTask(PRIORITY_HIGH));
  xTaskCreate(print_with_buttons, "print_with_buttons", 512, (void*) 1, 1, NULL );
  xTaskCreate(print_answer, "answer", 512, (void*) 1, 1, NULL );
  //  xTaskCreate(read_char, "read_char", 512, (void*) 1, 1, NULL);
  
  
  scheduler_start();
  
  return false;
}
