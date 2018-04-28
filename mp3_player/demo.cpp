#include "FreeRTOS.h"
#include "tasks.hpp"
#include "stdio.h"
#include <time.h>
#include <string.h>
#include "printf_lib.h"
#include "command_handler.hpp"
#include "event_groups.h"
#include "io.hpp"

#include "ssp1.h"
#include "gpioDriver.hpp"

LabGPIO mp3_dcs(0, 0);
LabGPIO mp3_cs(0, 1);
LabGPIO mp3_reset(2, 7);

void task (void* pvArgs)
{
	const uint8_t read = 0x03;
	const uint8_t write = 0x02;
	
	ssp1_init();
	ssp1_set_max_clock(2);

	// Init the 2 CS on the MP3 shield
	mp3_cs.setAsOutput();
	mp3_dcs.setAsOutput();
	mp3_cs.setHigh();
	mp3_dcs.setHigh();

	// Set dreq as input
	mp3_reset.setAsOutput();
	mp3_reset.setHigh();

	vTaskDelay(100);


	u0_dbg_printf("Reading mode register\n");
	mp3_cs.setLow();	
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x03));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x00));

	mp3_cs.setHigh();
	
	uint16_t tests = 0x6810;

	u0_dbg_printf("Writing mode\n");
	
	mp3_cs.setLow();
	
	// Set tests to be true
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x02));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x68));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x20));
	mp3_cs.setHigh();



	u0_dbg_printf("Reading mode register again\n");
	mp3_cs.setLow();
	
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x03));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x00));

	mp3_cs.setHigh();

	u0_dbg_printf("Sending test routine\n");
	
	vTaskDelay(100);
	mp3_dcs.setLow();
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x53));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0xEF));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x6E));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0xAA));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp1_exchange_byte(0x00));
	mp3_dcs.setHigh();

	while(1);
}


int main(int argc, char const *argv[])
{
	xTaskCreate(task, "task", 1024, NULL, 1, NULL);

	vTaskStartScheduler();
   
   return -1; 
}
