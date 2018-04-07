#include "FreeRTOS.h"
#include "tasks.hpp"
#include "stdio.h"
#include "printf_lib.h"
#include "io.hpp"
#include "command_handler.hpp"

#include "led_display_extension.hpp"

/*
 *  Made edits to:
 *     -  handlers.hpp
 *        - line 28: /// Extra credit for cmpe146 lab 6.
 *        - line 29: CMD_HANDLER_FUNC(orientationCmd);
 *     -  terminal.cpp
 *        - line 73: // cmpe146 Lab6 Extra Credit
 *        - line 74: cp.addHandler(orientationCmd,  "orientation", "Two options: 'orientation on' or 'orientation off'");
 */

#define LD_ARROWS      led_display_extension::getInstance()         ///< 2-Digit LED Display

enum orientation_t
{
  invalid = 0,
  up      = 1,
  down    = 2,
  left    = 3,
  right   = 4
};

struct producer_consumer_package{
  bool *  globalVar; 
};

orientation_t accelerometer; 
QueueHandle_t q;
TaskHandle_t producer_h;

void producer(void *p); /* LOW priority */
void consumer(void *p); /* HIGH priority */

CMD_HANDLER_FUNC(orientationCmd){
  // You can use FreeRTOS API or the wrapper resume() or suspend() methods
  if (cmdParams == "on")
    {
      //u0_dbg_printf("ON"); 
      vTaskResume(producer_h);
    }
  else
    {
      //output.putline("off");
      //u0_dbg_printf("OFF"); 
      vTaskSuspend(producer_h);
    }
  
  return true;
}

void producer(void *p) /* LOW priority */
{
  vTaskDelay(1000);
  int send = 0; 

  while (1) {
    int light_value = LS.getRawValue(); 
    //wait 1 second
    vTaskDelay(100);
  }
}

void consumer(void *p) /* HIGH priority */
{
  vTaskDelay(1000);
  const uint32_t one_second = 1000; 
  int x;
  while (1) {
    xQueueReceive(q, &x, one_second);
    u0_dbg_printf("recieved: %i\n", x);
  }
}

int main(int argc, char const *argv[])
{
  const uint32_t STACK_SIZE = 1024;
  
  // init the Acceleration Sensor
  AS.init();
  LD_ARROWS.init();
  
  // Queue handle is not valid until you create it
  q = xQueueCreate(10, sizeof(int));

  producer_consumer_package *package = new producer_consumer_package; 

  scheduler_add_task(new terminalTask(PRIORITY_HIGH));
  //  scheduler_add_task(new producer());
  //  scheduler_add_task(new consumer());
  xTaskCreate(producer, "producer", STACK_SIZE, (void *)package, 2 | portPRIVILEGE_BIT, &producer_h );
  xTaskCreate(consumer, "consumer", STACK_SIZE, (void *)package, 1 | portPRIVILEGE_BIT, NULL );

  scheduler_start();
  //  vTaskStartScheduler();

  return -1; 
}
