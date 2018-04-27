#include <stdio.h>
#include "task.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "LPC17xx.h"
#include "sempr.h"
#include "printf_lib.h"

#include "gpioStruct.hpp"
#include "gpioDriver.hpp"
#include "spiDriver.hpp"
using namespace std;

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
            //    button_push_flag = false;
            led_state = true;
        }
        if(*package->globalVar/*button_push_flag*/ == true && led_state == true){
            myGPIO_1_4->setHigh();
            *package->globalVar = false;
            //    button_push_flag = false;
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
                //      button_push_flag = true;
            }
            vTaskDelay(10);
        }
        last_button_state = button_state;
    }
    /* Only necessary if above loop has a condition */
    vTaskDelete(NULL);
}


int main(int argc, char const *argv[])
{
    
    const uint32_t STACK_SIZE = 1024;
    const uint8_t port = 0;
    const uint8_t pin = 6;
    
    //Declare SSP0
    LabSPI *mySSP0 = new LabSPI();
    mySSP0->init(mySSP0->SSP0, eight_bit, mySSP0->SP0, 4); //MOSI0
    mySSP0->init(mySSP0->SSP0, eight_bit, mySSP0->SP0, 2); //MISO0
    mySSP0->init(mySSP0->SSP0, eight_bit, mySSP0->SP0, 0); //CS0
    mySSP0->init(mySSP0->SSP0, eight_bit, mySSP0->SP0, 30); //SCLK0
    
    
    
    
    
    u0_dbg_printf("SSPI: %i, FORMAT: %i\n", mySSP1->SSP1, mySSP1->SPI);

    
    
    
    return 0;
}
