#include "pwmDriver.hpp"

void PWMDriver::pwmSelectAllPins()
{
	// Configure all possible pins as PWM
    LPC_PINCON->PINSEL4 |= (1<<0); // PWM1.1
	LPC_PINCON->PINSEL4	|= (1<<2); // PWM1.2
	LPC_PINCON->PINSEL4	|= (1<<4); // PWM1.3
	LPC_PINCON->PINSEL4	|= (1<<6); // PWM1.4
	LPC_PINCON->PINSEL4	|= (1<<8); // PWM1.5
	LPC_PINCON->PINSEL4	|= (1<<10); // PWM1.6
}

void PWMDriver::pwmSelectPin(PWM_PIN pwm_pin_arg)
{
	// Configure a specific pin as PWM
    switch(pwm_pin_arg)
    {
	case PWM_PIN_2_0:
		LPC_PINCON->PINSEL4 &= ~(3 << 0);
		LPC_PINCON->PINSEL4 |= (1 << 0);
        break;
		
	case PWM_PIN_2_1:
		LPC_PINCON->PINSEL4 &= ~(3 << 2);
		LPC_PINCON->PINSEL4 |= (1 << 2);
        break;
		
	case PWM_PIN_2_2:
		LPC_PINCON->PINSEL4 &= ~(3 << 4);
		LPC_PINCON->PINSEL4 |= (1 << 4);
        break;
		
	case PWM_PIN_2_3:
		LPC_PINCON->PINSEL4 &= ~(3 << 6);
		LPC_PINCON->PINSEL4 |= (1 << 6);
        break;
		
	case PWM_PIN_2_4:
		LPC_PINCON->PINSEL4 &= ~(3 << 8);
		LPC_PINCON->PINSEL4 |= (1 << 8);
        break;
		
	case PWM_PIN_2_5:
		LPC_PINCON->PINSEL4 &= ~(3 << 10);
		LPC_PINCON->PINSEL4 |= (1 << 10);
        break;
		
	default:
        break;
    }

}

void PWMDriver::pwmInitSingleEdgeMode(uint32_t frequency_Hz)
{
	const uint8_t pwm_bit = 6;
	const uint8_t pwm_clk = 12;
	const uint8_t prescale = 9;
	const uint32_t clk_speed = 1000000;
	
    // Turn on the PWM peripheral
	LPC_SC->PCONP |= (1 << pwm_bit);

	// Reset the peripheral clock
	LPC_SC->PCLKSEL0 &= ~(2 << pwm_clk);

	// Set up the clock
	// Set clock divider to 1
	LPC_SC->PCLKSEL0 |= (1  << pwm_clk);
    // Use a Prescale of 9 to make our clock 1MHz
	LPC_PWM1->PR = prescale;
	
	// 1 MHz / freq = size to put in MR0
	LPC_PWM1->MR0 = clk_speed / frequency_Hz;

	// Enable the counter
	LPC_PWM1->TCR = (1 << 0);

	// Cause a reset whenever counter = MR0 
	LPC_PWM1->MCR = (1 << 1);	

	// Enable pwm output on each channel
	LPC_PWM1->PCR |= (1 << 9);
	LPC_PWM1->PCR |= (1 << 10);
	LPC_PWM1->PCR |= (1 << 11);
	LPC_PWM1->PCR |= (1 << 12);
	LPC_PWM1->PCR |= (1 << 13);
	LPC_PWM1->PCR |= (1 << 14);

	// Enable the single edge on each channel
	LPC_PWM1->PCR |= (0 << 2);
	LPC_PWM1->PCR |= (0 << 3);
	LPC_PWM1->PCR |= (0 << 4);
	LPC_PWM1->PCR |= (0 << 5);
	LPC_PWM1->PCR |= (0 << 6);

	// Initially assign each match register to 0
	LPC_PWM1->MR1 = 0;
	LPC_PWM1->MR2 = 0;
	LPC_PWM1->MR3 = 0;
	LPC_PWM1->MR4 = 0;
	LPC_PWM1->MR5 = 0;
	LPC_PWM1->MR6 = 0;

	// Needed to reset the match register for each channel
	LPC_PWM1->LER |= (1 << 0);
	LPC_PWM1->LER |= (1 << 1);
	LPC_PWM1->LER |= (1 << 2);
	LPC_PWM1->LER |= (1 << 3);
	LPC_PWM1->LER |= (1 << 4);
	LPC_PWM1->LER |= (1 << 5);
	LPC_PWM1->LER |= (1 << 6);
}

void PWMDriver::setDutyCycle(PWM_PIN pwm_pin_arg, float duty_cycle_percentage)
{
	// To have n% duty multiply MR0 by n (duty_cycle_percentage)	
	const uint32_t MR0 = LPC_PWM1->MR0;
	const uint32_t cycles = (uint32_t) (duty_cycle_percentage * MR0);
	
	switch(pwm_pin_arg)
	{
	case PWM_PIN_2_0:
		LPC_PWM1->MR1 = cycles;
        LPC_PWM1->LER = (1 << 0);
        break;
		
	case PWM_PIN_2_1:
		LPC_PWM1->MR2 = cycles;
        LPC_PWM1->LER = (1 << 1);
        break;
		
	case PWM_PIN_2_2:
		LPC_PWM1->MR3 = cycles;
        LPC_PWM1->LER = (1 << 2);
        break;
		
	case PWM_PIN_2_3:
		LPC_PWM1->MR4 = cycles;
        LPC_PWM1->LER = (1 << 3);
        break;
		
	case PWM_PIN_2_4:
		LPC_PWM1->MR5 = cycles;
        LPC_PWM1->LER = (1 << 4);
        break;
		
	case PWM_PIN_2_5:
		LPC_PWM1->MR6 = cycles;
        LPC_PWM1->LER = (1 << 5);
        break;
		
	default:
		break;
	}
}

void PWMDriver::setFrequency(uint32_t frequency_Hz)
{
	const uint32_t clk_speed = 1000000;
	LPC_PWM1->MR0 = clk_speed / frequency_Hz;	
}
