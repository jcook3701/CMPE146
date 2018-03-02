#include "FreeRTOS.h"
#include "tasks.hpp"
#include "uart0_min.h"
#include "stdio.h"
#include "LPC17xx.h"

#include "adcDriver.hpp"
#include "pwmDriver.hpp"


const uint32_t ONE_SECOND = 1000;

float adc_reading;
float red_duty_cycle;
float blue_duty_cycle;
float green_duty_cycle;

void logVoltage( void* pvParameters )
{
	static char buffer[32];
	while(1)
	{
		const float voltage_percent = adc_reading;
		snprintf(buffer, 32, "Voltage: %fV", voltage_percent * 3.3);	
		uart0_puts(buffer);
		vTaskDelay(ONE_SECOND);
	}
}

void logDuty( void* pvParameters )
{
	static char buffer[64];
	while(1)
	{
		const float red   = red_duty_cycle;
		const float blue  = blue_duty_cycle;
		const float green = green_duty_cycle;
		
		snprintf(buffer, 64, "Duty: (%f, %f, %f)", red, green, blue);	
		uart0_puts(buffer);
		vTaskDelay(ONE_SECOND);
	}
}

void setColors( void* pvParameters )
{
	static PWMDriver pwm_driver;

	// Use these PWM pins
	pwm_driver.pwmSelectPin(PWMDriver::PWM_PIN_2_1); // Red
	pwm_driver.pwmSelectPin(PWMDriver::PWM_PIN_2_3); // Green
	pwm_driver.pwmSelectPin(PWMDriver::PWM_PIN_2_4); // Blue

	// 1000 Hz
	pwm_driver.pwmInitSingleEdgeMode(1000);
	
	while(1)
	{
		// Adapted from https://www.arduino.cc/en/Tutorial/ColorMixer
		if (adc_reading <= 0.33)
		{
			const float normalized = (adc_reading * 3);
			red_duty_cycle   = 1.0 - normalized;
			green_duty_cycle = normalized;
			blue_duty_cycle  = 1.0;
		}
		else if(adc_reading <= 0.66) 
		{
			const float normalized = ((adc_reading - 0.33) * 3);
			red_duty_cycle   = normalized;
			green_duty_cycle = 1.0;
			blue_duty_cycle  = 1.0 - normalized;
		}
		else
		{
			const float normalized = ((adc_reading - 0.67) * 3);
			red_duty_cycle = 1.0;
			green_duty_cycle = 1.0 - normalized;
			blue_duty_cycle = normalized;
		}

		
		pwm_driver.setDutyCycle(PWMDriver::PWM_PIN_2_1, red_duty_cycle);
		pwm_driver.setDutyCycle(PWMDriver::PWM_PIN_2_3, green_duty_cycle);
		pwm_driver.setDutyCycle(PWMDriver::PWM_PIN_2_4, blue_duty_cycle);
    }
}


void readADC(void * pvParameters)
{
	const uint8_t channel = 3;
	static ADCDriver adc_driver;
	
	adc_driver.adcInitBurstMode();
	adc_driver.adcSelectPin(ADCDriver::ADC_PIN_0_26);
	
	while(1)
	{
		adc_reading = adc_driver.readADCVoltageByChannel(channel);
    }
}

int main(int argc, char const *argv[])
{
	scheduler_add_task(new terminalTask(PRIORITY_HIGH));

	xTaskCreate(logVoltage, "VLOG", 512, (void*) 1, 1, NULL );
	xTaskCreate(logDuty, "DLOG", 512, (void*) 1, 1, NULL );
	xTaskCreate(readADC, "ADC", 512, (void * ) 1, 1, NULL );
	xTaskCreate(setColors, "PWM", 512, (void*) 1, 1, NULL );

	// Start Scheduler
	vTaskStartScheduler();

	return -1;
}
