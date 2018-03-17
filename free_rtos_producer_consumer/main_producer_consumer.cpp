#include "FreeRTOS.h"
#include "tasks.hpp"
#include "stdio.h"

#include "producer_consumer.hpp"

QueueHandle_t q;

void producer(void *p) /* LOW priority */
{
  int x = 0;
  while (1) {
    // This xQueueSend() will internally switch context over to the "consumer" task
    // because it is higher priority than this "producer" task
    // Then, when the consumer task sleeps, we will resume out of xQueueSend()
    // and go over to the next line
    xQueueSend(q, &x, 0);
    
    vTaskDelay(100);
    ++x;
  }
}

void consumer(void *p) /* HIGH priority */
{
  int x;
  while (1) {
    xQueueReceive(q, &x, portMAX_DELAY);
  }
}

int main(int argc, char const *argv[])
{
  const uint32_t STACK_SIZE = 1024;

  // Queue handle is not valid until you create it
  q = xQueueCreate(10, sizeof(int));

  producer_consumer_package *package = new producer_consumer_package; 
    
  xTaskCreate(producer, "producer", STACK_SIZE, (void *)package, 1 | portPRIVILEGE_BIT, NULL );
  xTaskCreate(consumer, "consumer", STACK_SIZE, (void *)package, 1 | portPRIVILEGE_BIT, NULL );


  return -1; 
}