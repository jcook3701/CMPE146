#include "FreeRTOS.h"
#include "tasks.hpp"
#include "printf_lib.h"
#include "io.hpp"
#include "ssp0.h"
#include "gpioDriver.hpp"
#include "LabGPIOInterrupts.hpp"
#include "ff.h"
#include "uart3.hpp"
#include <stdio.h>

/* Pin definitions */
LabGPIO mp3_dcs(0, 0);
LabGPIO mp3_cs(0, 1);
LabGPIO mp3_reset(2, 7);
LabGPIO mp3_dreq(2, 6);
LabGPIO on_board_1(1, 10);

// Buttons
LabGPIO btn_0(2, 0);
LabGPIO btn_1(2, 1);
LabGPIO btn_2(2, 2);
LabGPIO btn_3(2, 3);

/* For GPIO Interrupts */
LabGPIOInterrupts gpio_intr_instance;
void c_eint3_handler(void)
{
	gpio_intr_instance.handle_interrupt();
}

/* Semaphores for buttons */
SemaphoreHandle_t data_request = xSemaphoreCreateBinary();

/* The MP3 Data */
uint8_t buffer[512];

/* Queues */

enum actions
{
	next = 0,
	prev = 1,
	nope = 2,
};

QueueHandle_t buffer_ptr_queue = xQueueCreate(1, sizeof(uint32_t));
QueueHandle_t next_prev_control = xQueueCreate(1, sizeof(enum actions));


/* file paths stored here */
struct SongList
{
	SongList(void)
	{
		count = 0;
		current_index = 0;
		currently_playing = names[current_index];
	}
	void next(void)
	{
		current_index = (current_index + 1) % count;
		currently_playing = names[current_index];
	}
	void prev(void)
	{
		current_index = (current_index - 1) % count;
		currently_playing = names[current_index];
	}

	char* currently_playing;
	char names[16][50];
	uint8_t count;
	uint8_t current_index;
};

struct SongList song_list;

/* Misc/utility */
void populate_tracks_dir(void)
{
	FRESULT result;
	DIR dir;
	FILINFO finfo;

	uint8_t count = 0;
	// Open the directory containing the tracks
	result = f_opendir(&dir,"1:tracks");
	
	
	// While reading still functions
	while(FR_OK == f_readdir(&dir,&finfo))
	{
		// Still files in directory
		if(finfo.fname[0] != 0)
		{
			sprintf(song_list.names[count], "1:tracks/%s", finfo.fname);
			u0_dbg_printf("%d - %s\n",count, song_list.names[count]);
			count++;			
		}
		else
		{
			break;
		}
	}

	song_list.count = count;

	// Make sure to close the directory
	f_closedir(&dir);
}

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


/* Tasks */
class play_mp3 : public scheduler_task
{
public:
	play_mp3() : scheduler_task("play_mp3", 1024, PRIORITY_MEDIUM) {}
	
	bool init(void)
	{
		// Max 4 MHz
		ssp0_init(8);	
		
		// Set mode
		write_mp3_register(0x00, 0x08, 0x00);

		// Set clock multiplier on peripheral
		write_mp3_register(0x03, 0x60, 0x00);
		mp3_cs.setHigh();
		mp3_dcs.setHigh();
		return true;
	}
	
	bool run(void* pvArgs)
	{
		static unsigned int size;
		if(xQueueReceive(buffer_ptr_queue, &size, portMAX_DELAY))
		{
			mp3_dcs.setLow();
			for(int i = 0; i < size; ++i)
			{
				while(!mp3_dreq.getLevel());
				ssp0_exchange_byte(buffer[i]);
			}
			mp3_dcs.setHigh();
		}
		return true;
	}
};

class read_file : public scheduler_task
{
public:	
	read_file() : scheduler_task("read_file", 1024, PRIORITY_LOW)
	{
		done = false;
	}
	bool init(void)
	{
		open_song();
		return true;
	}
	bool open_song(void)
	{		
		result = f_open(&song, song_list.currently_playing,
						FA_READ | FA_OPEN_EXISTING);
		if(result == FR_OK)
		{
			u0_dbg_printf("Opened %s\n", song_list.currently_playing);
		}
		else
		{
			u0_dbg_printf("Could not open %s\n", song_list.currently_playing);
		}
		return true;
	}	
	bool run(void* pvArgs)
	{
		static enum actions action = nope;
		if(xQueueReceive(next_prev_control, &action, 10))
		{
			switch(action)
			{
			case next:
				f_close(&song);
				song_list.next();
				open_song();
				break;
			case prev:
				f_close(&song);
				song_list.prev();
				open_song();
				break;
			default:
				u0_dbg_printf("How\n");
			}
		}
		f_read(&song, buffer, 512, &read);
		xQueueSend(buffer_ptr_queue, &read, portMAX_DELAY);
		return true;
	}
		
private:
	FRESULT result;
	DIR dir;
	FILINFO finfo;
	FIL song;
	unsigned int read;
	bool done;
};	

void test(void)
{
	static int count = 0;
	u0_dbg_printf("btn pressed %d\n", ++count);
}

void play_isr(void)
{
	
}

void pause_isr(void)
{
	
}

void next_isr(void)
{
	const enum actions action = next;
	u0_dbg_printf("next\n");
	xQueueSend(next_prev_control, &action, 10);
}

void prev_isr(void)
{
	const enum actions action = prev;
	u0_dbg_printf("prev\n");
	xQueueSend(next_prev_control, &action, 10);	
}

void mp3_init(void)
{
	/* UART Init */
	Uart3& u3 = Uart3::getInstance();
	u3.init(9600);

	/* GPIO Init */
	mp3_cs.setAsOutput();
	mp3_dcs.setAsOutput();
	mp3_reset.setAsOutput();
	mp3_cs.setHigh();
	mp3_dcs.setHigh();
	mp3_reset.setHigh();
	mp3_dreq.setAsInput();  
	mp3_reset.setAsOutput();
	mp3_reset.setHigh();

	/* Button interrupts */
	gpio_intr_instance.init();
	gpio_intr_instance.attachInterruptHandler(2, 0, prev_isr, rising_edge);
	gpio_intr_instance.attachInterruptHandler(2, 1, test, rising_edge);
	gpio_intr_instance.attachInterruptHandler(2, 2, test, rising_edge);
	gpio_intr_instance.attachInterruptHandler(2, 3, next_isr, rising_edge);
	isr_register(EINT3_IRQn, c_eint3_handler);

	// Disable the flash memory
	LPC_PINCON->PINSEL0 &= ~(3 << 12); 
	LPC_GPIO0->FIODIR |= (1 << 6);
	LPC_GPIO0->FIOSET = (1 << 6);
}

int main(int argc, char const *argv[])
{

	populate_tracks_dir();
	mp3_init();
			
	scheduler_add_task(new play_mp3());
	scheduler_add_task(new read_file());
	//scheduler_add_task(new terminalTask(PRIORITY_HIGH));
	
	scheduler_start();
	return -1; 
}
