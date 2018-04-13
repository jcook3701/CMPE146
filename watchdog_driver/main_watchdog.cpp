#include "FreeRTOS.h"
#include "tasks.hpp"
#include "stdio.h"
#include <time.h>
#include <string.h>
#include "printf_lib.h"
#include "command_handler.hpp"
#include "event_groups.h"
#include "io.hpp"
#include "watchdogDriver.hpp"


#define STACK_SIZE 1024
#define watchdog_flag ( 1 << 0 )
#define producer_flag ( 1 << 1 )
#define consumer_flag ( 1 << 2 )
#define all_sync_bits ( watchdog_flag | producer_flag | consumer_flag)
/*
 *  Made edits to:
 *     -  handlers.hpp
 *        - line 28: /// Extra credit for cmpe146 lab 6.
 *        - line 29: CMD_HANDLER_FUNC(orientationCmd);
 *     -  terminal.cpp
 *        - line 73: // cmpe146 Lab6 Extra Credit
 *        - line 74: cp.addHandler(orientationCmd,  "orientation", "Two options: 'orientation on' or 'orientation off'");
 */

/*
struct producer_consumer_package{
  EventGroupHandle_t project_event_handler; 
};
*/

QueueHandle_t sensor_queue;

TaskHandle_t producer_handler;
TaskHandle_t consumer_handler;

EventGroupHandle_t event_handler; 

//void producer(void *p); /* MEDIUM priority */
//void consumer(void *p); /* MEDIUM priority */
//void watchdog(void *p); /* HIGH priority */ 

producer_task::producer_task() : scheduler_task("producer_task",  20 * 512, PRIORITY_MEDIUM)
{
}

bool producer_task::run(void *p)
{
  vTaskDelay(1000);
  
  uint8_t size = 100;
  uint8_t count = 0; 
  uint32_t sum = 0;
  uint32_t final = 0; 
  TickType_t xTicksToWait = 1000 / portTICK_PERIOD_MS;  //wait one second before acting
  while (1) {
    
    for(int i = 0; i < size; i ++){
      sum += LS.getRawValue();
      //wait 100ms second
      vTaskDelay(1);
    }
    final = sum/size;
    xQueueSend(sensor_queue, &final, 0);
    u0_dbg_printf("Producer: The average light value is: %i\n", final);
    //set producer flag bits
    xEventGroupSync(event_handler, producer_flag, all_sync_bits, xTicksToWait); 
  };
  return true; 
}

consumer_task::consumer_task() : scheduler_task("consumer_task",  20 * 512, PRIORITY_MEDIUM)
{
}

bool consumer_task::run(void *p)
{
  vTaskDelay(1000);  
  char word[128];
  char tmp[15]; 
  uint32_t received_value;
  bool value_was_received = false;
  TickType_t xTicksToWait = 1000 / portTICK_PERIOD_MS;  //wait one second before acting
  uint32_t count = 0; 
  
  while (1) {
    value_was_received = xQueueReceive(sensor_queue, &received_value, portMAX_DELAY);
    u0_dbg_printf("Consumer: recieved: %i\n", received_value);

    FILE *file0 = fopen("1:sensor_data.txt", "a");
    char line[128] = { 0 };
    //setup format for write to SD card. 
    strcpy(word,"time: ");
    snprintf(tmp, sizeof(((float)xTaskGetTickCount())/portTICK_PERIOD_MS), "%f",((float)xTaskGetTickCount())/portTICK_PERIOD_MS);
    strcat(word, tmp);
    strcat(word, " temp: ");
    snprintf(tmp, sizeof(received_value), "%lu", received_value);
    strcat(word, tmp);
    strcat(word, "\n");

    //    u0_dbg_printf("Consumer: word output: %s\n", word);
    //    u0_dbg_printf("Consumer: line output - read from sd card: %s\n\n", line);
    if(count % 10 == 0){
      /*
	because the producer only sends onece every 100ms if we wait ten times before writing
	a value to the sd card we will have waited 1 second inbetween writes. 
      */
      if(file0){
	fputs(word, file0);
	fgets(line, sizeof(line)-1, file0);
	fclose(file0);
      }
      //file0 = fopen("1:sensor_data.txt", "a");
      //if(file0){
      ///	fclose(file0);
      //}
      u0_dbg_printf("-----\n");
      u0_dbg_printf("Consumer: word output: %s\n", word);
      u0_dbg_printf("Consumer: read from sd card: %s\n", line);
      u0_dbg_printf("-----\n");
    }
    count++; 
    //set consumer flag bit
    xEventGroupSync(event_handler, consumer_flag, all_sync_bits, xTicksToWait); 
  };
  return true; 

}

watchdog_task::watchdog_task() : scheduler_task("watchdog_task",  8 * 512, PRIORITY_HIGH)
{
}

bool watchdog_task::run(void *p)
{
  vTaskDelay(1000);
  
  FILE *file0 = fopen("1:watchdog_cpu_info.txt", "a");
  FILE *file1 = fopen("1:watchdog_stuck_info.txt", "a");
  
  char word0[128];
  char word1[128];
  char line[128]  = { 0 };  
  char tmp[10];
  EventBits_t uxReturn;
  TickType_t xTicksToWait = 1000 / portTICK_PERIOD_MS;  //wait one second before acting
  bool one_second = false;
  uint32_t count = 0; 
  //printf("CPU Usage : %i %%\n", getTaskCpuPercent());    /* get OUR tasks' cpu usage */  needs to happen every 60 seconds.  
  
  while(1){
    /*
    if(xTaskGetTickCount()/portTICK_PERIOD_MS % 1000 == 0){
      one_second = true;  
      }*/
    uxReturn = xEventGroupSync(event_handler, watchdog_flag, all_sync_bits, xTicksToWait);
    if( ((uxReturn & all_sync_bits) == all_sync_bits)){
    }
    else{
      one_second = false; 
      if((uxReturn & producer_flag) != producer_flag){
	//then producer_flag is low on uxReturn
	strcpy(word1, "producer error!");
	u0_dbg_printf("watchdog: %s\n", word1);
	file1 = fopen("1:watchdog_stuck_info.txt", "a");
	if(file1){
	  fputs(word1, file1); 
	  //fgets(line, sizeof(line)-1, file1);
	  fclose(file1);
	}
      }
      if((uxReturn & consumer_flag) != consumer_flag){
	//then the consumer_flag is low on uxReturn
	strcpy(word1, "consumer error!");
	u0_dbg_printf("watchdog: %s\n", word1);
	file1 = fopen("1:watchdog_stuck_info.txt", "a");
	if(file1){
	  fputs(word1, file1); 
	  fgets(line, sizeof(line)-1, file1);
	  fclose(file1);
	}
      }
    }
    //    if(xTaskGetTickCount()/portTICK_PERIOD_MS % 60000 == 0){
    if(count == 6000){
      /* if count is equal to 6000 then the producer has run for a total time of 60,000ms or 60 secons */
      count = 0; 
      strcat(word0, "cpu: ");
      snprintf(tmp, sizeof(getTaskCpuPercent()), "%i\n", getTaskCpuPercent());
      strcat(word0, tmp);
      u0_dbg_printf("watchdog: %s\n", word0);
      file0 = fopen("1:watchdog_cpu_info.txt", "a");
      if(file0){
	fputs(word0, file0); 
	fgets(line, sizeof(line)-1, file0);
	fclose(file0);
      }
      count++; 
    }
    /* Both bit 0 and bit 1 are set if this if statment runs  */
  };
  return true; 
}

int main(int argc, char const *argv[])
{
  //const uint32_t STACK_SIZE = 1024;


  /* Init FreeRTOS features */
  // Queue handle is not valid until you create it
  sensor_queue = xQueueCreate(100, sizeof(uint32_t));
  event_handler = xEventGroupCreate();
  if( event_handler == NULL){
    u0_dbg_printf("The event group was not created because there was insufficent FreeRTOS heap available."); 
  }
  u0_dbg_printf("starting_up"); 


  //producer_consumer_package *package = new producer_consumer_package; 
  
  
  scheduler_add_task(new terminalTask(PRIORITY_HIGH));
  scheduler_add_task(new producer_task());
  scheduler_add_task(new consumer_task());
  scheduler_add_task(new watchdog_task()); 
  //xTaskCreate(producer, "producer", STACK_SIZE, (void *)package, 2 | portPRIVILEGE_BIT, &producer_handler );
  //xTaskCreate(consumer, "consumer", STACK_SIZE, (void *)package, 2 | portPRIVILEGE_BIT, &consumer_handler );
  //xTaskCreate(watchdog, "watchdog", STACK_SIZE, (void *)package, 1 | portPRIVILEGE_BIT, NULL );

  scheduler_start();
  //  vTaskStartScheduler();

  return -1; 
}
