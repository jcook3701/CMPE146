#include "FreeRTOS.h"
#include "task.h"
#include "uart0_min.h"
#include "printf_lib.h"

#include "LabGPIO_struct.hpp"
#include "LabGPIO.hpp"
#include "gpio_interrupt_driver.hpp"
#include "lpc_isr.h"
#include "semphr.h"

#define HIGH true
#define LOW false

SemaphoreHandle_t xSemaphore0 = 0;
//SemaphoreHandle_t xSemaphore1 = 0;

BaseType_t *watchPxHigherPriorityTaskWoken0;
//BaseType_t *watchPxHigherPriorityTaskWoken1;

/* global variabless */
bool button_push_flag0 = false; 
bool button_push_flag1 = false;

LabGPIOInterrupts gpio_intr_instance0;
//LabGPIOInterrupts gpio_intr_instance1; 

// This function will simply delegate the interrupt handling to our C++ class
// The CPU interrupt should be attached to this function through isr_register()
void c_eint3_handler(void)
{
  gpio_intr_instance0.handle_interrupt();
}

void user_callback0(void){
  xSemaphoreGiveFromISR(xSemaphore0, watchPxHigherPriorityTaskWoken0);
  u0_dbg_printf("test");
  button_push_flag0 = !button_push_flag0; 
  gpio_intr_instance0.clearInterruptFlag(); 
  //  LPC_GPIOINT->IO0IntClr |=(1<<0);
}

/*
void user_callback1(void){
  xSemaphoreGiveFromISR(xSemaphore1, watchPxHigherPriorityTaskWoken1);
  button_push_flag1 = !button_push_flag1;
  gpio_intr_instance1.clearInterruptFlag(); 
}
*/

void vControlLED( void * pvParameters )
{
  /* Get Parameter */
  //uint32_t param = (uint32_t)(pvParameters);
  GPIO_Package *package;
  package = (GPIO_Package*) pvParameters;
  
  /* Define Constants Here */
  
  /* Define Local Variables and Objects */
  //LabGPIO_1 *myGPIO_1_4 = new  LabGPIO_1(package->pin); 
  LabGPIO *myGPIO_1_4 = new LabGPIO(package->port, package->pin); 
  bool led_state = false;
  
  /* Initialization Code */
  myGPIO_1_4->setAsOutput();
  //myGPIO_1_4->setLow();
  while(1)
    {
      if(xSemaphoreTake(xSemaphore0, portMAX_DELAY)){
	//u0_dbg_printf("hello world");
	if(*package->globalVar/*button_push_flag*/== true && led_state == false){
	  myGPIO_1_4->setLow();
	  *package->globalVar = false;
	  //	button_push_flag = false;
	  led_state = true; 
	}
	if(*package->globalVar/*button_push_flag*/ == true && led_state == true){
	  myGPIO_1_4->setHigh();
	  *package->globalVar = false;
	  //	button_push_flag = false;
	  led_state = false; 
	}
      }
    }
  /* Only necessary if above loop has a condition */
  vTaskDelete(NULL);
}

// You can comment out the sample code of lpc1758_freertos project and run this code instead
int main(int argc, char const *argv[])
{
  /// This "stack" memory is enough for each task to run properly
  const uint32_t STACK_SIZE = 1024;
  u0_dbg_printf("stack size: %d!\n", STACK_SIZE);
  uart0_puts("testing");

  /* Define Local Variables and Objects */
  GPIO_Package * vControl_LED_package0 = new GPIO_Package; 
  //GPIO_Package * vControl_LED_package1 = new GPIO_Package; 
  
  /* Initialization Code */
  xSemaphore0 = xSemaphoreCreateBinary();
  gpio_intr_instance0.init(c_eint3_handler);
  gpio_intr_instance0.attachInterruptHandler(0, 0, user_callback0, falling_edge);
  //0, 0
  /*
  xSemaphore1 = xSemaphoreCreateBinary();
  gpio_intr_instance1.init(c_eint3_handler);
  gpio_intr_instance1.attachInterruptHandler(2, 7, user_callback1, falling_edge); 
  */

  //----------------internal LED & switch---------------------//
  vControl_LED_package0->port = 1;
  vControl_LED_package0->pin = 4;
  vControl_LED_package0->globalVar = &button_push_flag0;

  /*
  vControl_LED_package1->port = 1;
  vControl_LED_package1->pin = 0;
  vControl_LED_package1->globalVar = &button_push_flag1; 
  */

  xTaskCreate(vControlLED, "LED0_P1.4", STACK_SIZE, (void *)vControl_LED_package0, 1, NULL);
  //xTaskCreate(vControlLED, "LED0_P1.9", STACK_SIZE, (void *)vControl_LED_package0, 1, NULL); 
  /* Start Scheduler - This will not return, and your tasks will start to run their while(1) loop */
  vTaskStartScheduler();
    
  return 0;
}

/*
void vReadSwitch( void * pvParameters )
{
  // Get Parameter /
  //uint32_t param = (uint32_t)(pvParameters);
  GPIO_Package *package;
  package = (GPIO_Package*) pvParameters;
  
  // Define Constants Here /
  const uint8_t pin = package->pin;
 
  // Define Local Variables and Objects /
  //LabGPIO_1 *myGPIO_1_9 = new LabGPIO_1(pin);
  LabGPIO *myGPIO_1_9 = new LabGPIO(package->port, package->pin); 
  
  //----------------Variables for Debouncing --------------------------//
  int button_push_counter = 0; // counter for the number of button presses
  int button_state = 0;        // current state of the button
  int last_button_state = 0;   // previous state of the button
  
  // Initialization Code /
  myGPIO_1_9->setAsInput();
  
  while(1)
    {
      button_state = myGPIO_1_9->getLevel(); //Reading the input pin. 
      // compare the buttonState to its previous state
      if(button_state != last_button_state){
	if(button_state == LOW) {
	  button_push_counter++;
	  *package->globalVar = true; 
	  //	  button_push_flag = true;
	}
	vTaskDelay(50);
      }
      last_button_state = button_state; 
    }
  // Only necessary if above loop has a condition /
  vTaskDelete(NULL);
}
*/
