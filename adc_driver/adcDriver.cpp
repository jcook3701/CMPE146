#include "adcDriver.hpp"

// Bit 12 in LPC_SRC->PCONP turns on/off the ADC
static const uint8_t adc_pin = 12;

static const uint8_t burst_mode_pin = 16;

// Bits 15:4 contain the value in the adc register
static const uint32_t ADC_VALUE_MASK = 0x0000FFF0;

ADCDriver::ADCDriver() {};

void ADCDriver::adcInitBurstMode()
{
	// Power up the ADC
	LPC_SC->PCONP |= (1 << adc_pin);		

	// Divide the clock by 1 (default is 4)
	// Bits 25:24 configure the peripheral clock spd for ADCdriver
	// 25:24 = 01 -> CLK/1
	LPC_SC->PCLKSEL0 |= (0 << 25);
	LPC_SC->PCLKSEL0 |= (1 << 24);

	// Enable burst mode
	LPC_ADC->ADCR |= (1 << burst_mode_pin);
}

void ADCDriver::adcSelectPin(ADCDriver::ADC_PIN adc_pin_arg)
{
	// Bits 7:0 of LPC_ADC->ADCR selects  which channel to read
	// If bit n is set then read from channel n
	// All other bits should be 0 (hence the mask)
	LPC_ADC->ADCR &= (0xFFFFFF00);
	
	switch(adc_pin_arg)
	{
	case ADC_PIN_0_25:
		// Light sensor on the SJOne Board
		LPC_PINCON->PINSEL1 |= (0 << 19);
		LPC_PINCON->PINSEL1 |= (1 << 18);
		LPC_ADC->ADCR |= (1 << 2);
		break;
		
	case ADC_PIN_0_26:
		LPC_PINCON->PINSEL1 |= (0 << 21);
		LPC_PINCON->PINSEL1 |= (1 << 20);
		LPC_ADC->ADCR |= (1 << 3);
		break;

	case ADC_PIN_1_30:
		LPC_PINCON->PINSEL3 |= (1 << 29);
		LPC_PINCON->PINSEL3 |= (1 << 28);
		LPC_ADC->ADCR |= (1 << 4);
		break;
		
	case ADC_PIN_1_31:
		LPC_PINCON->PINSEL3 |= (1 << 31);
		LPC_PINCON->PINSEL3 |= (1 << 30);
		LPC_ADC->ADCR |= (1 << 5);
		break;
		
	default:
		break;
	}
}

float ADCDriver::readADCVoltageByChannel(uint8_t adc_channel_arg)
{
	// 15:4 = 12bit adc reading
	uint16_t raw_value;
	
	switch(adc_channel_arg)
	{
	case 0:
		raw_value = (uint16_t) LPC_ADC->ADDR0;
		break;
		
	case 1:
		raw_value = (uint16_t) LPC_ADC->ADDR1;
		break;

	case 2:
		raw_value = (uint16_t) LPC_ADC->ADDR2;
		break;
		
	case 3:
		raw_value = (uint16_t) LPC_ADC->ADDR3;
		break;
		
	case 4:
		raw_value = (uint16_t) LPC_ADC->ADDR4;
		break;
		
	case 5:
		raw_value = (uint16_t) LPC_ADC->ADDR5;
		break;

	case 6:
		raw_value = (uint16_t) LPC_ADC->ADDR6;
		break;
		
	case 7:
		raw_value = (uint16_t) LPC_ADC->ADDR7;
		break;
		
	default:
		raw_value = 0xFFFF;
		break;
	}

	// Shift 15:4 to 11:0 for the 12 bit value
	const uint16_t shifted = raw_value >> 4;

	// Put the return value in a range of 0 to 1
	// 2^12 = 4096
	const float ret_val = shifted / 4096.0;
	return ret_val;
}
