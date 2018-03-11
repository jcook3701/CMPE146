#ifndef LABSPI_H
#define LABSPI_H

#include "LPC17xx.h"

class LabSPI
{
private:
  // SSP register lookup table structure

  //LPC_SSP_TypeDef original_ssp[2] = {LPC_SSP0, LPC_SSP1};
  LPC_SSP_TypeDef * SSP[2] = {LPC_SSP0, LPC_SSP1}; 
  uint8_t current_ssp = 0; 
  
public:
  enum FrameModes
    {
      /* Fill this out based on the datasheet. */
      SPI       = 0,
      TI        = 1,
      Microwire = 2
      //MODE0 - CPOL: 0 | CPHA: 0
      //MODE1 - CPOL: 0 | CPHA: 1
      //MODE2 - CPOL: 1 | CPHA: 0
      //MODE3 - CPOL: 1 | CPHA: 1
      //      MODE0,
      //      MODE1,
      //      MODE2,
      //      MODE3
    };
  
  enum Peripheral
    {
      SSP0 = 0,
      SSP1 = 1
    };
  
  /**
   * 1) Powers on SPPn peripheral
   * 2) Set peripheral clock
   * 3) Sets pins for specified peripheral to MOSI, MISO, and SCK
   *
   * @param peripheral which peripheral SSP0 or SSP1 you want to select.
   * @param data_size_select transfer size data width; To optimize the code, look for a pattern in the datasheet
   * @param format is the code format for which synchronous serial protocol you want to use.
   * @param divide is the how much to divide the clock for SSP; take care of error cases such as the value of 0, 1, and odd numbers
   *
   * @return true if initialization was successful
   */
  bool init(Peripheral peripheral, uint8_t data_size_select, FrameModes format, uint8_t divide);
  
  /**
   * Transfers a byte via SSP to an external device using the SSP data register.
   * This region must be protected by a mutex static to this class.
   *
   * @return received byte from external device via SSP data register.
   */
  uint8_t transfer(uint8_t send);
  
  LabSPI();
  ~LabSPI();
};

#endif
