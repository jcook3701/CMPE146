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

void task_sig_reader(void *p)
{
  const uint8_t expected1 = 0x1F;
  const uint8_t expected2 = 0x26; 
  
  /* Get Parameter */
  GPIO_SPI_Package *package;
  package = (GPIO_SPI_Package*) p;
  
  while (1)
  {
    // Note that we may get interrupted here and the other task may corrupt our transfer
    package->gpio_object->setLow();
    uint8_t s1 = package->spi_object->transfer(0x9F); // FIXME: This is not the real command to read signature
    uint8_t s2 = package->spi_object->transfer(0x00);
    uint8_t s3 = package->spi_object->transfer(0x00);
    package->gpio_object->setHigh();
    
    if (s2 != expected1 || s3 != expected2) {
      u0_dbg_printf("Ooops... race condition");
      vTaskSuspend(NULL); // Suspend this task
    }
    else{
      u0_dbg_printf("Manufacturer ID: %x\n", s2);
      u0_dbg_printf("Device ID: %x\n", s3);
    }
    //vTaskDelay(1);
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
    for(int i = 0; i < page_size; i++){
      read_page[i] = package->spi_object->transfer(0x0);
    }
    package->gpio_object->setHigh();
    for(int i = 0; i < page_size; i++){
      u0_dbg_printf("page read %x\n", read_page[i]);
    }
    vTaskDelay(1);
  }
}

int main(int argc, char const *argv[])
{
  const uint32_t STACK_SIZE = 1024;
  const uint8_t port = 0;
  const uint8_t pin = 6; 

  // Declare SSP1
  LabSPI *mySSP1 = new LabSPI();
  mySSP1->init(mySSP1->SSP1, eight_bit , mySSP1->SPI, 4);
  u0_dbg_printf("SSPI: %i, FORMAT: %i\n", mySSP1->SSP1, mySSP1->SPI);
  
  // Declare GPIO
  LabGPIO *myGPIO_0_6 = new LabGPIO(port, pin);
  myGPIO_0_6->setAsOutput();
  
  // Declare Package
  GPIO_SPI_Package * spi_package = new GPIO_SPI_Package; 
  
  /* Initialization Code */
  //----------------internal GPIO, SSP1---------------------//
  spi_package->globalVar = &active_flag0;
  spi_package->spi_object = mySSP1;
  spi_package->gpio_object = myGPIO_0_6; 

  xTaskCreate(task_sig_reader, "sig_reader", STACK_SIZE, (void *)spi_package, 1 | portPRIVILEGE_BIT, NULL );
  xTaskCreate(task_page_reader, "page_reader", STACK_SIZE, (void *)spi_package, 1 | portPRIVILEGE_BIT, NULL );

  // Start Scheduler
  vTaskStartScheduler();

  return -1;
}
