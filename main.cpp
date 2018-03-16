#include "FreeRTOS.h"
#include "tasks.hpp"
#include "uart0_min.h"
#include "stdio.h"
#include "LPC17xx.h"

#include "uartDriver.hpp"

LabUART UART2;

QueueHandle_t charQueue;

void print_name(void* pvparams)
{
	char x;
	char c;

	char myname[] = {'p', 'e', 't', 'e', 'r'};
	char i = 0;
	
	while(1)
	{
	
		UART2.transmit(i);
		//x = UART2.receive();
		//printf("%c: %x\n", x, x);
		i++;
		//vTaskDelay(100);
	}
}

int main(int argc, char const *argv[])
{
	UART2.init();
	
	scheduler_add_task(new terminalTask(PRIORITY_HIGH));
	xTaskCreate(print_name, "print", 512, (void*) 1, 1, NULL );
	scheduler_start();
	
	return false;
}
