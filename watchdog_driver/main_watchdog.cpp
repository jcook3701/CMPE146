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


struct producer_consumer_package{
  EventGroupHandle_t project_event_handler; 
};


QueueHandle_t sensor_queue;

TaskHandle_t producer_handler;
TaskHandle_t consumer_handler;

EventGroupHandle_t event_handler; 

void producer(void *p); /* MEDIUM priority */
void consumer(void *p); /* MEDIUM priority */
void watchdog(void *p); /* HIGH priority */ 

producer_task::producer_task() : scheduler_task("producer_task", STACK_SIZE, PRIORITY_MEDIUM)
{
}

bool producer_task::run(void *p)
{
    vTaskDelay(1);

  producer_consumer_package *package;
  package = (producer_consumer_package*) p;
  
  uint8_t size = 100;
  uint8_t count = 0; 
  uint32_t sum = 0;
  uint32_t final = 0; 
    
  while (1) {
    
    if(count == size-1){
      final = sum/size; 
      u0_dbg_printf("Producer: The average light value is: %i\n", final);
      xQueueSend(sensor_queue, &final, 0);
      count = 0;
    }
    sum += LS.getRawValue(); 
    count++;
    //set producer flag bit
    xEventGroupSetBits(package->project_event_handler, producer_flag);

    //wait 1 second
    vTaskDelay(1);
  };
  return true; 
}

consumer_task::consumer_task() : scheduler_task("consumer_task", STACK_SIZE, PRIORITY_MEDIUM)
{
}

bool consumer_task::run(void *p)
{
  vTaskDelay(1000);

  producer_consumer_package *package;
  package = (producer_consumer_package*) p;
  
  char line[128] ;
  char word[128];
  char tmp[10]; 
  uint32_t received_value;
  bool value_was_received = false;

  while (1) {
    value_was_received = xQueueReceive(sensor_queue, &received_value, portMAX_DELAY);
    u0_dbg_printf("Consumer: recieved: %i\n", received_value);

    FILE *file0 = fopen("1:sensor_data.txt", "a");
    //setup format for write to SD card. 
    strcpy(word,"time: ");
    snprintf(tmp, sizeof(((float)xTaskGetTickCount())/portTICK_PERIOD_MS), "%f",((float)xTaskGetTickCount())/portTICK_PERIOD_MS);
    strcat(word, tmp);
    strcat(word, " tempature: ");
    snprintf(tmp, sizeof(received_value), "%lu", received_value);
    strcat(word, tmp);
    u0_dbg_printf("Consumer: word output: %s\n", word);
    if(file0){
	fputs(word, file0); 
	fgets(line, sizeof(line)-1, file0);
	fclose(file0);
      }
    u0_dbg_printf("Consumer: line output - read from sd card: %s\n\n", line);
    //set consumer flag bit
    xEventGroupSetBits(package->project_event_handler, consumer_flag);
    
  };
  return true; 

}

watchdog_task::watchdog_task() : scheduler_task("watchdog_task", STACK_SIZE, PRIORITY_HIGH)
{
}

bool watchdog_task::run(void *p)
{
  printf("\n---------------------------------\n");
	 
  //Package holdes event
  producer_consumer_package *package;
  package = (producer_consumer_package*) p;
  
  FILE *file0 = fopen("1:watchdog_info.txt", "a");

  TickType_t xTicksToWait = 1000 / portTICK_PERIOD_MS;  //wait one second before acting

  while(1){

    EventBits_t uxReturn = xEventGroupSync(package->project_event_handler, watchdog_flag, all_sync_bits, xTicksToWait);
    if((uxReturn & all_sync_bits) == all_sync_bits){

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
  producer_consumer_package *package = new producer_consumer_package; 
  
  
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
