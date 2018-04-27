#include "spiDriver.hpp"
#include "gpioDriver.hpp"


LabSPI::LabSPI(){};

bool LabSPI::init(Peripheral peripheral, uint8_t data_size_select, FrameModes format, uint8_t divide)
{
  bool return_val = false;
  
  if ( data_size_select % 2 == 0 && data_size_select != 0 ){
    //Sets the SSP peripheral for this object
    this->current_ssp = peripheral; 


    // LPC_SC->PCONP bits
    const uint8_t ssp0_bit   = 21;  
    const uint8_t ssp1_bit   = 10;

    // LPC_SC->PCLKSEL bits
    const uint8_t ssp0_clk   = 10;  // LPC_SC->PCLKSEL1 - pins 11:10
    const uint8_t ssp1_clk   = 20;  // LPC_SC->PCLKSEL0 - pins 21:20
    
    //ssp0
    //const uint8_t gpio_cs0   = 0;   //LPC_PINCON->PINSEL1 - pins 1:0
    const uint8_t sclk0      = 30;  //LPC_PINCON->PINSEL0 - pins 31:30
    const uint8_t miso0      = 2;   //LPC_PINCON->PINSEL1 - pins 3:2
    const uint8_t mosi0      = 4;   //LPC_PINCON->PINSEL1 - pins 5:4
    
    //ssp1
    //const uint8_t gpio_cs1   = 12;  //LPC_PINCON->PINSEL0 - pins 13:12
    const uint8_t sclk1      = 14;  //LPC_PINCON->PINSEL0 - pins 15:14
    const uint8_t miso1      = 16;  //LPC_PINCON->PINSEL0 - pins 17:16
    const uint8_t mosi1      = 18;  //LPC_PINCON->PINSEL0 - pins 19:18

    // Functionality for SSP pins
    const uint8_t clear_two_bits = 0x3; 
    //const uint8_t gpio_fun       = 0x0;
    const uint8_t sclk_fun       = 0x2; 
    const uint8_t miso_fun       = 0x2;
    const uint8_t mosi_fun       = 0x2;
    
    switch(peripheral)
      {
      case SSP0:
	// Turn on the SSP0 peripheral
	LPC_SC->PCONP |= (0x1 << ssp0_bit);
	// Reset the peripheral clock
	LPC_SC->PCLKSEL1 &= ~(0x3 << ssp0_clk);
	// Set up the clock
	// Set clock divider to 1 (PCLK_peripheral = CCLK)
	LPC_SC->PCLKSEL1 |= (0x2  << ssp0_clk);
	//--------------------SSP0 Pin Select Select Registers-------------------------//
	//sets - cs0 to GPIO pin.  
	//LPC_PINCON->PINSEL1 &= ~(clear_two_bits << gpio_cs0);
	//LPC_PINCON->PINSEL1 |=  (gpio_fun << gpio_cs0);
	//clears then sets sclk0
	LPC_PINCON->PINSEL0 &= ~(clear_two_bits << sclk0);
	LPC_PINCON->PINSEL0 |=  (sclk_fun << sclk0);  
	//clears then sets miso0
	LPC_PINCON->PINSEL1 &= ~(clear_two_bits << miso0);
	LPC_PINCON->PINSEL1 |=  (miso_fun << miso0);
	//clears then sets mosi0
	LPC_PINCON->PINSEL1 &= ~(clear_two_bits << mosi0);
	LPC_PINCON->PINSEL1 |=  (mosi_fun << mosi0);
	break;
      case SSP1:
	// Turn on the SSP1 peripheral
	LPC_SC->PCONP |= (0x1 << ssp1_bit);
	// Reset the peripheral clock
	LPC_SC->PCLKSEL0 &= ~(0x3 << ssp1_clk);
	// Set up the clock
	// Set clock divider to 1
	LPC_SC->PCLKSEL0 |= (0x2  << ssp1_clk);
	//--------------------SSP1 Pin Select Select Registers-------------------------//
	//sets - cs0 to GPIO pin.  
	//LPC_PINCON->PINSEL0 &= ~(clear_two_bits << gpio_cs1);
	//LPC_PINCON->PINSEL0 |=  (gpio_fun << gpio_cs1);
	//clears then sets sclk0
	LPC_PINCON->PINSEL0 &= ~(clear_two_bits << sclk1);
	LPC_PINCON->PINSEL0 |=  (sclk_fun << sclk1);  
	//clears then sets miso0
	LPC_PINCON->PINSEL0 &= ~(clear_two_bits << miso1);
	LPC_PINCON->PINSEL0 |=  (miso_fun << miso1);
	//clears then sets mosi0
	LPC_PINCON->PINSEL0 &= ~(clear_two_bits << mosi1);
	LPC_PINCON->PINSEL0 |=  (mosi_fun << mosi1);
	break;
      }
    
    //-----------------Control registers--------------------------//
    const uint8_t dss = 0;
    const uint8_t clock_prescale_register = 0; 
    const uint8_t frame_format = 4;
    //const uint8_t cpol = 6;
    //const uint8_t cpha = 7;

    //---------------Contrl Register 1-----------------//
    //Data Size Select
    SSP[peripheral]->CR0 |=   (data_size_select << dss);    
    //Frame Format - pins 5:4
    SSP[peripheral]->CR0 &=  ~(0x3 << frame_format);  //Clear - or set to 00 (SPI)
    SSP[peripheral]->CR0 |=  (format << frame_format);  //Clear - or set to 00 (SPI)
    
    const uint8_t lbm = 0;
    const uint8_t sse = 1;
    const uint8_t ms  = 2;

    // Contrl Register 1
    SSP[peripheral]->CR1 &= ~(0x1 << lbm);  //Sets to 0
    SSP[peripheral]->CR1 &= ~(0x1 << sse);  //Clears bit
    SSP[peripheral]->CR1 |=  (0x1 << sse);  //Sets to 1
    SSP[peripheral]->CR1 &= ~(0x1 << ms);   //Sets to 0

    //SSPn Clock Prescale Register
    SSP[peripheral]->CPSR &= ~(0x7F << clock_prescale_register);
    SSP[peripheral]->CPSR |=  (divide << clock_prescale_register);

    return_val = true; 
  } 
  return return_val; 
}

uint8_t LabSPI::transfer(uint8_t send)
{

  //SSP[this->current_ssp]->DR &= ~(0xFF << 0);
  //what is the difference between (SSP[this->current_ssp]->DR = send; &   SSP[this->current_ssp]->DR |= (send << 0); )
  SSP[this->current_ssp]->DR = send;
  while(SSP[this->current_ssp]->SR & (1 << 4)){}
  return (SSP[this->current_ssp]->DR);
  
  //  SSP[this->current_ssp]->DR |= (send << 0);
  //  while(SSP[this->current_ssp]->SR & (1 << 4)){
  //    return SSP[this->current_ssp]->DR;
  //}
  
}

adlx_t LabSPI::getStatusReg()
{
  adlx_t current_status;
  current_status.TFE = (SSP[this->current_ssp]->SR & 0b1);
  current_status.TNF = (SSP[this->current_ssp]->SR >> 1 & 0b1);
  current_status.RNE = (SSP[this->current_ssp]->SR >> 2 & 0b1);
  current_status.RFE = (SSP[this->current_ssp]->SR >> 3 & 0b1);
  current_status.BSY = (SSP[this->current_ssp]->SR >> 4 & 0b1);
  return current_status; 
}


