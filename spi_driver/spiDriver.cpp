#include "spiDriver.hpp"
#include "gpioDriver.hpp"

LabSPI::LabSPI(){};


bool LabSPI::init(Peripheral peripheral, uint8_t data_size_select, FrameModes format, uint8_t divide)
{
  const uint8_t ssp0_bit = 21;
  const uint8_t ssp1_bit = 10;
  const uint8_t ssp0_clk = 12;
  const uint8_t ssp1_clk = 10;

  //ssp0
  const uint8_t cs0   = 0;   //PCLKSEL1 - pins 1:0
  const uint8_t sclk0 = 30;  //PCLKSEL0 - pins 31:30
  const uint8_t miso0 = 2;   //PCLKSEL1 - pins 3:2
  const uint8_t mosi0 = 4;   //PCLKSEL1 - pins 5:4
  
  //ssp1
  const uint8_t cs1   = 12;  //PCLKSEL0 - pins 13:12
  const uint8_t sclk1 = 14;  //PCLKSEL0 - pins 15:14
  const uint8_t miso1 = 16;  //PCLKSEL0 - pins 17:16
  const uint8_t mosi1 = 18;  //PCLKSEL0 - pins 19:18
  
  bool return_val = false; 

  if(peripheral == SSP0)
    {
      // Turn on the SSP0 peripheral
      LPC_SC->PCONP |= (0x1 << ssp0_bit);

      // Reset the peripheral clock
      LPC_SC->PCLKSEL0 &= ~(0x3 << ssp0_clk);
      
      // Set up the clock
      // Set clock divider to 1
      LPC_SC->PCLKSEL0 |= (0x1  << ssp0_clk);

      //--------------------SSP0 Select Registers-------------------------//
      //sets - cs0 to GPIO pin.  
      LPC_PINCON->PINSEL1 &= ~(0x3 << cs0);
      //clears then sets sclk0
      LPC_PINCON->PINSEL0 &= ~(0x3 << sclk0);
      LPC_PINCON->PINSEL0 |=  (0x2 << sclk0);  
      //clears then sets miso0
      LPC_PINCON->PINSEL1 &= ~(0x3 << miso0);
      LPC_PINCON->PINSEL1 |=  (0x2 << miso0);
      //clears then sets mosi0
      LPC_PINCON->PINSEL1 &= ~(0x3 << mosi0);
      LPC_PINCON->PINSEL1 |=  (0x2 << mosi0);
    }
  if(peripheral == SSP1)
    {
      // Turn on the SSP1 peripheral
      LPC_SC->PCONP |= (1 << ssp1_bit);
      
      // Reset the peripheral clock
      LPC_SC->PCLKSEL1 &= ~(2 << ssp1_clk);

      // Set up the clock
      // Set clock divider to 1
      LPC_SC->PCLKSEL0 |= (1  << ssp1_clk);

      //--------------------SSP1 Select Registers-------------------------//
      //sets - cs0 to GPIO pin.  
      LPC_PINCON->PINSEL0 &= ~(0x3 << cs1);
      //clears then sets sclk0
      LPC_PINCON->PINSEL0 &= ~(0x3 << sclk1);
      LPC_PINCON->PINSEL0 |=  (0x2 << sclk1);  
      //clears then sets miso0
      LPC_PINCON->PINSEL0 &= ~(0x3 << miso1);
      LPC_PINCON->PINSEL0 |=  (0x2 << miso1);
      //clears then sets mosi0
      LPC_PINCON->PINSEL0 &= ~(0x3 << mosi1);
      LPC_PINCON->PINSEL0 |=  (0x2 << mosi1);

    }
  
  return return_val; 

}

uint8_t LabSPI::transfer(uint8_t send)
{

  return -1; 
}

