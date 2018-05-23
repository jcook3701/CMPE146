#include "LabGPIOInterrupts.hpp"

LabGPIOInterrupts::LabGPIOInterrupts() {}

void LabGPIOInterrupts::init()
{	
	NVIC_EnableIRQ(EINT3_IRQn);
}

bool LabGPIOInterrupts::attachInterruptHandler(uint8_t port,
											   uint32_t pin,
											   void (*pin_isr)(void),
											   InterruptCondition_E condition)
{
	if((port == 0) && (pin > 30))
	{
		return false;
	}
	if((port == 2) && (pin > 10))
	{
		return false;
	}
	
	// Set up the port/pin as in interrupt
	if(condition == rising_edge || condition == both_edges)
	{
		if(port == 0)
		{
			LPC_GPIOINT->IO0IntEnR |= (1 << pin);
		}
		else
		{
			LPC_GPIOINT->IO2IntEnR |= (1 << pin);
		}
	}
	if(condition == falling_edge || condition == both_edges)
	{
		if(port == 0)
		{
			LPC_GPIOINT->IO0IntEnF |= (1 << pin);
		}
		else
		{
			LPC_GPIOINT->IO2IntEnF |= (1 << pin);
		}
	}
	
	const uint32_t row = (port == 0) ? 0 : 1;
	this->isrs[row][pin].isr        = pin_isr;
	this->isrs[row][pin].condition  = condition;
	this->isrs[row][pin].pin        = pin;
	this->isrs[row][pin].port       = port;
	this->registered.push_back(&isrs[row][pin]);

	return true;
}

bool check_pin(uint32_t pin, uint32_t reg)
{
	return ((reg & (1 << pin)) != 0);
}

void handle_isr(isr_vector* vec, uint32_t r)
{
	const uint32_t port = vec->port;
	const uint32_t pin = vec->pin;
	
	if(check_pin(pin, r))
	{
		vec->isr();

		if(port == 0)
		{
			LPC_GPIOINT->IO0IntClr |= (1 << pin);
		}
		else
		{
			LPC_GPIOINT->IO2IntClr |= (1 << pin);
		}
	}
}

void LabGPIOInterrupts::handle_interrupt(void)
{
	const uint32_t rising_pins0  = LPC_GPIOINT->IO0IntStatR;
	const uint32_t falling_pins0 = LPC_GPIOINT->IO0IntStatF;
	const uint32_t rising_pins2  = LPC_GPIOINT->IO2IntStatR;
	const uint32_t falling_pins2 = LPC_GPIOINT->IO2IntStatF;

	// Iterate through the registered ISRS
	for (uint32_t i = 0; i < registered.size(); ++i)
	{
		isr_vector* x = registered[i];
		
		if(x->port == 0)
		{
			if(x->condition == rising_edge || x->condition == both_edges)
			{
				handle_isr(x, rising_pins0);
			}
			if(x->condition == falling_edge || x->condition == both_edges)
			{				
				handle_isr(x, falling_pins0);
			}
		}
		else
		{
			if(x->condition == rising_edge || x->condition == both_edges)
			{
				handle_isr(x, rising_pins2);
			}
			if(x->condition == falling_edge || x->condition == both_edges)
			{				
				handle_isr(x, falling_pins2);
			}		   			
		}
	}
}

LabGPIOInterrupts::~LabGPIOInterrupts() {}

