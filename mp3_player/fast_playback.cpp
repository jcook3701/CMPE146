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
#include "ff.h"

LabGPIO mp3_dcs(0, 0);
LabGPIO mp3_cs(0, 1);
LabGPIO mp3_reset(2, 7);
LabGPIO mp3_reset2(2, 5);
LabGPIO mp3_dreq(2, 6);

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

void mp3SIRead(uint8_t addr)
{
	mp3_cs.setLow();
	vTaskDelay(2);
	ssp0_exchange_byte(0x03);
	ssp0_exchange_byte(addr);
	u0_dbg_printf("%0.2x", ssp0_exchange_byte(0xFF));
	u0_dbg_printf("%0.2x\n", ssp0_exchange_byte(0xFF));
	vTaskDelay(2);
	mp3_cs.setHigh();
}

void mp3SIWrite(uint8_t addr, uint8_t high, uint8_t low)
{
	mp3_cs.setLow();
	vTaskDelay(2);
	ssp0_exchange_byte(0x02);
	ssp0_exchange_byte(addr);
	u0_dbg_printf("%0.2x", ssp0_exchange_byte(high));
	u0_dbg_printf("%0.2x\n", ssp0_exchange_byte(low));
	vTaskDelay(2);
	mp3_cs.setHigh();
}

void play_mp3_routine (void* pvArgs)
{
	uint8_t buffer[512];

	FRESULT result;
	DIR dir;
	FILINFO finfo;
	FIL song;
	size_t read;

	ssp0_init(12);
/*	LPC_SC -> PCONP |= (1<<10);
	LPC_SC -> PCLKSEL1 &= ~(3<<10);
	LPC_SC->PCLKSEL1 |=  (1 << 11); // CLK / 2

	LPC_SSP0->CR0 = 7;          // 8-bit mode
	LPC_SSP0->CR1 = (1 << 1);   // Enable SSP as Master
	LPC_SSP0->CPSR = 8;*/

		
	// Init the 2 CS on the MP3 shield
	mp3_cs.setAsOutput();
	mp3_dcs.setAsOutput();
	mp3_reset.setAsOutput();
	mp3_reset2.setAsOutput();

	mp3_cs.setHigh();
	mp3_dcs.setHigh();
	mp3_reset.setHigh();
	mp3_reset2.setHigh();
	
	mp3_dreq.setAsInput();
   

	// Set dreq as input
	mp3_reset.setAsOutput();
	mp3_reset.setHigh();
	
	vTaskDelay(100);

	/*
	mp3_cs.setLow();	
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x02));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x08));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));
	mp3_cs.setHigh();
	*/

	// Write Mode register
	mp3SIWrite(0x00, 0x08, 0x00);
	u0_dbg_printf(" \n \n Reg 0 (Mode Control 0x800): \n");
	mp3SIRead(0x00);

	// Read status register
	u0_dbg_printf("Reg 1 (Status): \n");
	mp3SIRead(0x01);

	// Adjust clock register to 4MHz
	mp3SIWrite(0x03, 0x00, 0x00);
	u0_dbg_printf("Reg 3 (CLOCK Freq 0x6000): \n");
	mp3SIRead(0x03);

	u0_dbg_printf("Reg 4 (Decode Time): \n");
	mp3SIRead(0x04);

	u0_dbg_printf("Reg 5 (Aux Data, 0x1f40): \n");
	mp3SIRead(0x05);
	
	// Open the song
	result = f_open(&song, "1:tracks/track004.mp3", FA_READ | FA_OPEN_EXISTING);
	if(result == FR_OK)
	{
		u0_dbg_printf("Opened file\n",finfo.fname);	
	}
	else
	{
		u0_dbg_printf("Could not open file\n",finfo.fname);	
	}

	mp3_cs.setHigh();
	mp3_dcs.setLow();
	while(f_eof(&song) == 0)
	{
		if(mp3_dreq.getLevel() == true)
		{
			f_read(&song, buffer, 512, &read);
			for(int i = 0; i < read; ++i)
			{
				ssp0_exchange_byte(buffer[i]);
			}
		}
	}
	mp3_dcs.setHigh();

	u0_dbg_printf("done\n");
	
	// Make sure to close the directory	
	vTaskSuspend(0);
}

void sine_test_routine (void* pvArgs)
{
	const uint8_t read = 0x03;
	const uint8_t write = 0x02;
	
	ssp0_init(1);
	ssp0_set_max_clock(2);

	print_tracks_dir();
	
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
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x03));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));

	mp3_cs.setHigh();
	
	uint16_t tests = 0x6810;

	u0_dbg_printf("Writing mode\n");
	
	mp3_cs.setLow();
	
	// Set tests to be true
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x02));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x68));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x20));
	mp3_cs.setHigh();



	u0_dbg_printf("Reading mode register again\n");
	mp3_cs.setLow();
	
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x03));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));

	mp3_cs.setHigh();

	u0_dbg_printf("Sending test routine\n");
	
	vTaskDelay(100);
	mp3_dcs.setLow();
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x53));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0xEF));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x6E));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0xAA));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));
	u0_dbg_printf("%x\n", ssp0_exchange_byte(0x00));
	mp3_dcs.setHigh();


	vTaskSuspend(0);
	while(1);
}

int main(int argc, char const *argv[])
{	
	//disable
	LPC_PINCON->PINSEL0 &= ~(3 << 12); // p0.6
	LPC_GPIO0->FIODIR |= (1 << 6); // p0.6
	LPC_GPIO0->FIOSET = (1 << 6); // SET HIGH forever to disable FLASH mem.
	
	//xTaskCreate(sine_test_routine, "task", 1024, NULL, 1, NULL);
	xTaskCreate(play_mp3_routine, "task", 4096, NULL, 1, NULL);
	vTaskStartScheduler();   
	return -1; 
}
