#include "FreeRTOS.h"
#include "tasks.hpp"
#include "uart0_min.h"
#include "stdio.h"
#include "LPC17xx.h"
#include "lpc_isr.h"

#include "uartDriver.hpp"

LabUART UART2;

QueueHandle_t charQueue;

void print_char(void* pvparams)
{
	char i = 0;
	
	while(1)
	{	
		UART2.transmit('5');
		UART2.transmit('4');
		UART2.transmit('+');
		vTaskDelay(10000);
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
	int i= 0;
	char input[3] = { 0 };
	int answer;
	
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
				UART2.transmit(answer + '0');
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
			printf("Sender received: %c\n", c);
		}
	}
}

void rx_int_handler(void)
{
	char c;
	c = UART2.receive();
	xQueueSend(charQueue, &c, 1000);
}

int main(int argc, char const *argv[])
{
	UART2.init();

	isr_register(UART2_IRQn, rx_int_handler);
	NVIC_EnableIRQ(UART2_IRQn);

	charQueue = xQueueCreate(100, sizeof(char));
	
	scheduler_add_task(new terminalTask(PRIORITY_HIGH));
	xTaskCreate(print_char, "print_char", 512, (void*) 1, 1, NULL );
	xTaskCreate(print_answer, "answer", 512, (void*) 1, 1, NULL );
	xTaskCreate(read_char, "read_char", 512, (void*) 1, 1, NULL);



	scheduler_start();
	
	return false;
}
