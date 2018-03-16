#include "FreeRTOS.h"
#include "tasks.hpp"
#include "uart0_min.h"
#include "stdio.h"
#include "LPC17xx.h"

#include "uartDriver.hpp"

LabUART UART2;

QueueHandle_t charQueue;

#define ALU 1


void print_name(void* pvparams)
{
	char i = 0;
	
	while(1)
	{
	
		UART2.transmit(i);
		i++;
		vTaskDelay(100);
	}
}

void read_name(void* pvparams)
{
	char x;
	
	while(1)
	{
		x = UART2.receive();
		printf("%c: %x\n", x, x);
		vTaskDelay(100);
	}
}

int main(int argc, char const *argv[])
{
	UART2.init();
	
	scheduler_add_task(new terminalTask(PRIORITY_HIGH));

	#if ALU
	printf("ALU Task\n");
	xTaskCreate(print_name, "print_name", 512, (void*) 1, 1, NULL );	

	#else
	printf("Sender Task\n");
	xTaskCreate(read_name, "send_name", 512, (void*) 1, 1, NULL);
	#endif
	
	scheduler_start();
	
	return false;
}
