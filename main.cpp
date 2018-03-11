#include "FreeRTOS.h"
#include "tasks.hpp"
#include "LPC17xx.h"
#include "stdio.h"
#include "printf_lib.h"


#include "gpioStruct.hpp"
#include "gpioDriver.hpp"
#include "spiDriver.hpp"


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

void cs(void){
  
}

void ds(void){
  
}

void task_sig_reader(void *p)
{
  /* Get Parameter */
  //uint32_t param = (uint32_t)(pvParameters);
  GPIO_SPI_Package *package;
  package = (GPIO_SPI_Package*) p;
  
  while (1)
  {

    // Note that we may get interrupted here and the other task may corrupt our transfer
    package->gpio_object->setLow();
    char s1 = package->spi_object->transfer(0x9F); // FIXME: This is not the real command to read signature
    char s2 = package->spi_object->transfer(0x00);
    char s3 = package->spi_object->transfer(0x00);
    package->gpio_object->setHigh();
    u0_dbg_printf("Manufacturer ID: %x\n", s2);
    u0_dbg_printf("Device ID: %x\n", s3);

    /*
    if (s1 != expected1 || s2 != expected2) {
      puts("Ooops... race condition");
      vTaskSuspend(NULL); // Suspend this task
    }
    */
    vTaskDelay(1);
  }
}

void task_page_reader(void *p)
{
  while (1)
  {
    cs();
    //read_512_byte_page();
    ds();
    vTaskDelay(1);
  }
}

int main(int argc, char const *argv[])
{
  const uint32_t STACK_SIZE = 1024;
  const uint8_t port = 0;
  const uint8_t pin = 0; 


  // Declare SSP1
  LabSPI *mySSP1 = new LabSPI();
  mySSP1->init(mySSP1->SSP0, sixteen_bit , mySSP1->SPI, 8);
  u0_dbg_printf("SSPI: %i, FORMAT: %i\n", mySSP1->SSP1, mySSP1->SPI);
  
  // Declare GPIO
  LabGPIO *myGPIO_0_6 = new LabGPIO(port, pin);
  myGPIO_0_6->setAsOutput();
  
  // Declare Package
  GPIO_SPI_Package * chip_select = new GPIO_SPI_Package; 
  
  /* Initialization Code */
  //----------------internal GPIO, SSP1---------------------//
  chip_select->globalVar = &active_flag0;
  chip_select->spi_object = mySSP1;
  chip_select->gpio_object = myGPIO_0_6; 


  xTaskCreate(task_sig_reader, "sig_reader", STACK_SIZE, (void *)chip_select, 1, NULL );
  //xTaskCreate(task_page_reader, "page_reader", STACK_SIZE, (void *)chip_select, 1, NULL );

  // Start Scheduler
  vTaskStartScheduler();

  return -1;
}
