#include "FreeRTOS.h"
#include "tasks.hpp"
#include "stdio.h"
#include <time.h>
#include <string.h>
#include "printf_lib.h"
#include "command_handler.hpp"
#include "event_groups.h"
#include "io.hpp"

#include "ssp0.h"
#include "gpioDriver.hpp"
#include "LabGPIOInterrupts.hpp"
#include "ff.h"
#include "uart3.hpp"

LabGPIO mp3_dcs(0, 0);
LabGPIO mp3_cs(0, 1);
LabGPIO mp3_reset(2, 7);
LabGPIO mp3_dreq(2, 6);
LabGPIO on_board_1(1, 10);

LabGPIOInterrupts gpio_intr_instance;

// This function will simply delegate the interrupt handling to our C++ class
// The CPU interrupt should be attached to this function through isr_register()
void c_eint3_handler(void)
{
	gpio_intr_instance.handle_interrupt();
}

SemaphoreHandle_t button_sem = xSemaphoreCreateBinary();

void print_tracks_dir(void)
{
	FRESULT result;
	DIR dir;
	FILINFO finfo;

	// Open the directory containing the tracks
	result = f_opendir(&dir,"1:tracks");

	// While reading still functions
	while(FR_OK == f_readdir(&dir,&finfo))
	{
		// Still files in directory
		if(finfo.fname[0] != 0)
		{			
			u0_dbg_printf("%s\n",finfo.fname);
		}
		else
		{
			break;
		}
	}

	// Make sure to close the directory
	f_closedir(&dir);
}

void read_mp3_register(uint8_t addr)
{
	static const uint8_t read_op = 0x03;
	static const uint8_t noop = 0xff;
	
	mp3_cs.setLow();
	ssp0_exchange_byte(read_op);
	ssp0_exchange_byte(addr);	
	const uint8_t msb = ssp0_exchange_byte(noop);
	const uint8_t lsb = ssp0_exchange_byte(noop);
	mp3_cs.setHigh();

	u0_dbg_printf("%0.2x%0.2x\n", msb, lsb);
}

void write_mp3_register(uint8_t addr, uint8_t msb, uint8_t lsb)
{
	static const uint8_t write_op = 0x02;
	
	mp3_cs.setLow();
	ssp0_exchange_byte(write_op);
	ssp0_exchange_byte(addr);
	ssp0_exchange_byte(msb);
	ssp0_exchange_byte(lsb);
	mp3_cs.setHigh();
}

void play_mp3_routine(void* pvArgs)
{
	uint8_t buffer[512];

	FRESULT result;
	DIR dir;
	FILINFO finfo;
	FIL song;
	size_t read;
	
	// Write Mode register
	write_mp3_register(0x00, 0x08, 0x00);
	u0_dbg_printf(" \n \n Reg 0 (Mode Control 0x800): \n");
	read_mp3_register(0x00);

	// Read status register
	u0_dbg_printf("Reg 1 (Status): \n");
	read_mp3_register(0x01);

	// Adjust clock register to 4MHz
	write_mp3_register(0x03, 0x60, 0x00);
	u0_dbg_printf("Reg 3 (CLOCK Freq 0x6000): \n");
	read_mp3_register(0x03);

	u0_dbg_printf("Reg 4 (Decode Time): \n");
	read_mp3_register(0x04);

	u0_dbg_printf("Reg 5 (Aux Data, 0x1f40): \n");
	read_mp3_register(0x05);
	
	// Open the song
	result = f_open(&song, "1:tracks/track010.mp3", FA_READ | FA_OPEN_EXISTING);
	if(result == FR_OK)
	{
		u0_dbg_printf("Opened file\n",finfo.fname);	
	}
	else
	{
		u0_dbg_printf("Could not open file\n",finfo.fname);	
	}

	mp3_cs.setHigh();
	while(f_eof(&song) == 0)
	{
		f_read(&song, buffer, 512, &read);
		mp3_dcs.setLow();
		for(int i = 0; i < read; ++i)
		{
			while(!mp3_dreq.getLevel());
			ssp0_exchange_byte(buffer[i]);
		}
		mp3_dcs.setHigh();
	
	}

	u0_dbg_printf("done\n");
	
	// Make sure to close the directory	
	vTaskSuspend(0);
}

void print(void* pvArgs)
{
	Uart3& u3 = Uart3::getInstance();
	u0_dbg_printf("Test\n");
	u3.putline("test\n");
	
	while(1)
	{
		if(xSemaphoreTake(button_sem, 1000))
		{
			u0_dbg_printf("Button\n");
		}
		
	}
}

void handle1()
{
	xSemaphoreGiveFromISR(button_sem, NULL);
}

void mp3_init(void)
{
	// Configure the pins and peripherals
	ssp0_init(4);		
	mp3_cs.setAsOutput();
	mp3_dcs.setAsOutput();
	mp3_reset.setAsOutput();
	mp3_cs.setHigh();
	mp3_dcs.setHigh();
	mp3_reset.setHigh();
	mp3_dreq.setAsInput();  
	mp3_reset.setAsOutput();
	mp3_reset.setHigh();

	//gpio_intr_instance.init();
	//gpio_intr_instance.attachInterruptHandler(2, 6, handle1, falling_edge);
	//isr_register(EINT3_IRQn, c_eint3_handler);


	// Disable the flash memory
	LPC_PINCON->PINSEL0 &= ~(3 << 12); // p0.6
	LPC_GPIO0->FIODIR |= (1 << 6); // p0.6
	LPC_GPIO0->FIOSET = (1 << 6); // SET HIGH forever to disable FLASH mem.
}

int main(int argc, char const *argv[])
{
	Uart3& u3 = Uart3::getInstance();
	u3.init(9600);
	mp3_init();
		
	//xTaskCreate(sine_test_routine, "task", 1024, NULL, 1, NULL);
	xTaskCreate(play_mp3_routine, "task", 4096, NULL, 1, NULL);
	xTaskCreate(print, "task2", 1024, NULL, 1, NULL);
	vTaskStartScheduler();   
	return -1; 
}
