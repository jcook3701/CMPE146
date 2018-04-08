#include "FreeRTOS.h"
#include "task.h"
#include "uart0_min.h"
#include "printf_lib.h"
#include "lpc_isr.h"
#include "semphr.h"

#include "gpioStruct.hpp"
#include "gpioDriver.hpp"

#define HIGH true
#define LOW false

SemaphoreHandle_t button_press_semaphore = NULL;
/* global variabless */
bool button_push_flag0 = false; 
bool button_push_flag1 = false;

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
  /* Only necessary if above loop has a condition */
  vTaskDelete(NULL);
}

void vReadSwitch( void * pvParameters )
{
  /* Get Parameter */
  //uint32_t param = (uint32_t)(pvParameters);
  GPIO_Package *package;
  package = (GPIO_Package*) pvParameters;
  
  /* Define Constants Here */
  const uint8_t pin = package->pin;
 
  /* Define Local Variables and Objects */
  //LabGPIO_1 *myGPIO_1_9 = new LabGPIO_1(pin);
  LabGPIO *myGPIO_1_9 = new LabGPIO(package->port, package->pin); 
  
  //----------------Variables for Debouncing --------------------------//
  int button_push_counter = 0; // counter for the number of button presses
  int button_state = 0;        // current state of the button
  int last_button_state = 0;   // previous state of the button
  
  /* Initialization Code */
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
	vTaskDelay(10);
      }
      last_button_state = button_state; 
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
  GPIO_Package * vRead_Switch_package0 = new GPIO_Package; 
  GPIO_Package * vControl_LED_package0 = new GPIO_Package; 

  GPIO_Package * vRead_Switch_package1 = new GPIO_Package; 
  GPIO_Package * vControl_LED_package1 = new GPIO_Package; 

  //isr_register(EINT3_IRQn , );
  /* Initialization Code */
  //----------------internal LED & switch---------------------//
  vRead_Switch_package0->port = 1; 
  vRead_Switch_package0->pin = 14;
  vRead_Switch_package0->globalVar = &button_push_flag0;

  vControl_LED_package0->port = 1; 
  vControl_LED_package0->pin = 4;
  vControl_LED_package0->globalVar = &button_push_flag0; 
  
  //----------------external LED & switch---------------------//
  vRead_Switch_package1->port = 1; 
  vRead_Switch_package1->pin = 20;
  vRead_Switch_package1->globalVar = &button_push_flag1;

  vControl_LED_package1->port = 0; 
  vControl_LED_package1->pin = 0;
  vControl_LED_package1->globalVar = &button_push_flag1;

  
  //----------------internal LED & switch---------------------//
  xTaskCreate(vReadSwitch, "Button0_P1.14", STACK_SIZE, (void *)vRead_Switch_package0, 1, NULL);
  xTaskCreate(vControlLED, "LED0_P1.4", STACK_SIZE, (void *)vControl_LED_package0, 1, NULL);
  //----------------external LED & switch---------------------//
  xTaskCreate(vReadSwitch, "Button0_P1.19", STACK_SIZE, (void *)vRead_Switch_package1, 1, NULL);
  xTaskCreate(vControlLED, "LED0_P1.20", STACK_SIZE, (void *)vControl_LED_package1, 1, NULL);

  
  /* Start Scheduler - This will not return, and your tasks will start to run their while(1) loop */
  vTaskStartScheduler();
    
  return 0;
}
