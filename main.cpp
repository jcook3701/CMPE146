#include "FreeRTOS.h"
#include "tasks.hpp"
#include "uart0_min.h"
#include "LPC17xx.h"
#include "stdio.h"
#include "adcDriver.hpp"

int main(int argc, char const *argv[])
{
	ADCDriver adc = ADCDriver();
	adc.adcSelectPin(ADCDriver::ADC_PIN_0_25);
	adc.adcInitBurstMode();	
	
	char buf[32];	
	while(1)
	{
		snprintf(buf, 512, "PIN: %u\n", adc.readADCVoltageByChannel(2));
		uart0_puts(buf);
	}	
    return -1;
}
