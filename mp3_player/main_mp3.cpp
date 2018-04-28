#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <stdint.h>
#include "LPC17xx.h"
#include "semphr.h"
#include "printf_lib.h"

#include "gpioStruct.hpp"
#include "gpioDriver.hpp"
#include "spiDriver.hpp"
using namespace std;

SemaphoreHandle_t button_press_semaphore = NULL;

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
            if(button_state == false) {
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
//Semaphore
SemaphoreHandle_t spi_bus_lock = xSemaphoreCreateMutex();

//Data Sizes
const uint8_t four_bit      = 0x3;
const uint8_t five_bit      = 0x4;
const uint8_t six_bit       = 0x5;
const uint8_t seven_bit     = 0x6;
const uint8_t eight_bit     = 0x7;
const uint8_t nine_bit      = 0x8;
const uint8_t ten_bit       = 0x9;
const uint8_t eleven_bit    = 0xA;
const uint8_t twelve_bit    = 0xB;
const uint8_t thirteen_bit  = 0xC;
const uint8_t fourteen_bit  = 0xD;
const uint8_t fifteen_bit   = 0xE;
const uint8_t sixteen_bit   = 0xF;

bool active_flag0 = false;
adlx_t status_reg;

void task_sig_reader(void *p)
{
    const uint8_t expected1 = 0x1F;
    const uint8_t expected2 = 0x26;
    
    /* Get Parameter */
    GPIO_SPI_Package *package;
    package = (GPIO_SPI_Package*) p;
    
    while (1)
    {
        if(xSemaphoreTake(spi_bus_lock, 1))
        {
            // Use Guarded Resource
            // Note that we may get interrupted here and the other task may corrupt our transfer
            package->gpio_object->setLow();
            uint8_t s1 = package->spi_object->transfer(0x9F);
            uint8_t s2 = package->spi_object->transfer(0x00);
            uint8_t s3 = package->spi_object->transfer(0x00);
            package->gpio_object->setHigh();
            
            if (s2 != expected1 || s3 != expected2)
            {
                u0_dbg_printf("Ooops... race condition\n");
                u0_dbg_printf("Manufacturer ID: %x\n", s2);
                u0_dbg_printf("Device ID: %x\n", s3);
                vTaskSuspend(NULL); // Suspend this task
            }
            else
            {
                u0_dbg_printf("Manufacturer ID: %x\n", s2);
                u0_dbg_printf("Device ID: %x\n", s3);
            }
            // Give Semaphore back:
            xSemaphoreGive(spi_bus_lock);
        }
        vTaskDelay(1);
    }
}

void task_page_reader(void *p)
{
    GPIO_SPI_Package *package;
    package = (GPIO_SPI_Package*) p;
    
    uint8_t page_size = 5;
    uint8_t read_page[page_size];
    while (1)
    {
        if(xSemaphoreTake(spi_bus_lock, 1))
        {
            // Use Guarded Resource
            package->gpio_object->setLow();
            //read_512_byte_page();
            uint8_t s1 = package->spi_object->transfer(0xD2);
            uint8_t s2 = package->spi_object->transfer(0x0);  // Address byte
            uint8_t s3 = package->spi_object->transfer(0x0);  // Address byte
            uint8_t s4 = package->spi_object->transfer(0x0);  // Address byte
            uint8_t s5 = package->spi_object->transfer(0x0);  // dummy byte
            uint8_t s6 = package->spi_object->transfer(0x0);  // dummy byte
            uint8_t s7 = package->spi_object->transfer(0x0);  // dummy byte
            uint8_t s8 = package->spi_object->transfer(0x0);  // dummy byte
            for(int i = 0; i < page_size; i++)
            {
                read_page[i] = package->spi_object->transfer(0x0);
            }
            package->gpio_object->setHigh();
            for(int i = 0; i < page_size; i++)
            {
                u0_dbg_printf("page read %i: %x\n",i, read_page[i]);
            }
            // Give Semaphore back:
            xSemaphoreGive(spi_bus_lock);
        }
        vTaskDelay(1);
    }
}


int main(int argc, char const *argv[])
{
    
    const uint32_t STACK_SIZE = 1024;
    const uint8_t port = 0;
    const uint8_t pin = 6;
    
    //Declare SSP0//
    LabSPI *mySSP0 = new LabSPI();
    mySSP0->init(mySSP0->SSP0, eight_bit, mySSP0->SPI, 4); //MOSI0
    mySSP0->init(mySSP0->SSP0, eight_bit, mySSP0->SPI, 2); //MISO0
    mySSP0->init(mySSP0->SSP0, eight_bit, mySSP0->SPI, 0); //CS0
    mySSP0->init(mySSP0->SSP0, eight_bit, mySSP0->SPI, 30); //SCLK0
    
    
    //GPIO Initialization//
    GPIO_Package * vRead_Switch_package0 = new GPIO_Package;
    GPIO_Package * vRead_Switch_package1 = new GPIO_Package;
    
    //P1.30, mp3_dreq
    vRead_Switch_package0->port = 1;
    vRead_Switch_package0->pin = 30;
    vRead_Switch_package0->globalVar = &button_push_flag0;

    //P1.29, mp3_rst
    vRead_Switch_package0->port = 1;
    vRead_Switch_package0->pin = 29;
    
    //P1.28, led_backlit_control
    vRead_Switch_package0->port = 1;
    vRead_Switch_package0->pin = 28;
    
    //P1.23, led_enable
    vRead_Switch_package0->port = 1;
    vRead_Switch_package0->pin = 23;
    
    //P1.22, led_rs_rw
    vRead_Switch_package0->port = 1;
    vRead_Switch_package0->pin = 22;

    //P1.20, led_d7
    vRead_Switch_package0->port = 1;
    vRead_Switch_package0->pin = 20;
    
    //P1.19, led_d6
    vRead_Switch_package0->port = 1;
    vRead_Switch_package0->pin = 19;
    
    //P0.30, led_d5
    vRead_Switch_package0->port = 0;
    vRead_Switch_package0->pin = 30;
    
    //P0.29, led_d4
    vRead_Switch_package0->port = 0;
    vRead_Switch_package0->pin = 29;
    
    //P2.0, led_backlit_control
    vRead_Switch_package0->port = 2;
    vRead_Switch_package0->pin = 0;
    
    //P0.1, mp3_cs
    vRead_Switch_package0->port = 0;
    vRead_Switch_package0->pin = 1;
    
    //P0.0, mp3_dcs
    vRead_Switch_package0->port = 0;
    vRead_Switch_package0->pin = 0;
    
    
    
    //    u0_dbg_printf("SSPI: %i, FORMAT: %i\n", mySSP1->SSP1, mySSP1->SPI);

    
    xTaskCreate(task_sig_reader, "sig_reader", STACK_SIZE, (void *)spi_package, 1 | portPRIVILEGE_BIT, NULL );
    xTaskCreate(task_page_reader, "page_reader", STACK_SIZE, (void *)spi_package, 1 | portPRIVILEGE_BIT, NULL );
    
    //xTaskCreate(vReadSwitch, "SW", STACK_SIZE, (void *)vRead_Switch_package0, 1, NULL);
    //xTaskCreate(vControlLED, "LED", STACK_SIZE, (void *)vControl_LED_package0, 1, NULL);
    
    
    /* Start Scheduler - This will not return, and your tasks will start to run their while(1) loop */
    vTaskStartScheduler();

    
    
    
    return 0;
}
