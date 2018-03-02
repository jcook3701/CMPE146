#include "pwmDriver.hpp"

//bit 6 in LPCSRC->PCONP turns on/off the pwm
static const uint8_t pwm_pin = 6; 

void PWMDriver::pwmSelectAllPins()
{
  LPC_SC->PCONP |= (0x1 << pwm_pin); 

  //PWM_PIN_2_0
  LPC_PINCON->PINSEL3 &= ~(0x3 << 4);
  LPC_PINCON->PINSEL3 |= (0x2 << 4); 
  //PWM_PIN_2_1
  LPC_PINCON->PINSEL3 &= ~(0x3 << 8);
  LPC_PINCON->PINSEL3 |= (0x2 << 8); 
  //PWM_PIN_2_2
  LPC_PINCON->PINSEL3 &= ~(0x3 << 10);
  LPC_PINCON->PINSEL3 |= (0x2 << 10);
  //PWM_PIN_2_3
  LPC_PINCON->PINSEL3 &= ~(0x3 << 14);
  LPC_PINCON->PINSEL3 |= (0x2 << 14);
  //PWM_PIN_2_4
  LPC_PINCON->PINSEL3 &= ~(0x3 << 16);
  LPC_PINCON->PINSEL3 |= (0x2 << 16); 
  //PWM_PIN_2_5
  LPC_PINCON->PINSEL3 &= ~(0x3 << 20);
  LPC_PINCON->PINSEL3 |= (0x2 << 20);
}

void PWMDriver::pwmSelectPin(PWMDriver::PWM_PIN pwm_pin_arg)
{
  LPC_SC->PCONP |= (0x1 << pwm_pin);   
  switch(pwm_pin_arg)
    {
    case PWM_PIN_2_0:
      LPC_PINCON->PINSEL3 &= ~(0x3 << 4);
      LPC_PINCON->PINSEL3 |= (0x2 << 4); 
      break;
    case PWM_PIN_2_1:
      LPC_PINCON->PINSEL3 &= ~(0x3 << 8);
      LPC_PINCON->PINSEL3 |= (0x2 << 8); 
      break;
    case PWM_PIN_2_2:
      LPC_PINCON->PINSEL3 &= ~(0x3 << 10);
      LPC_PINCON->PINSEL3 |= (0x2 << 10); 
      break;
    case PWM_PIN_2_3:
      LPC_PINCON->PINSEL3 &= ~(0x3 << 14);
      LPC_PINCON->PINSEL3 |= (0x2 << 14); 
      break;
    case PWM_PIN_2_4:
      LPC_PINCON->PINSEL3 &= ~(0x3 << 16);
      LPC_PINCON->PINSEL3 |= (0x2 << 16); 
      break;
    case PWM_PIN_2_5:
      LPC_PINCON->PINSEL3 &= ~(0x3 << 20);
      LPC_PINCON->PINSEL3 |= (0x2 << 20); 
      break; 
    }
}

void PWMDriver::pwmInitSingleEdgeMode(uint32_t frequency_Hz)
{
  
}

void PWMDriver::setDutyCycle(PWMDriver::PWM_PIN pwm_pin_arg, float duty_cycle_percentage)
{
  
}

void PWMDriver::setFrequency(uint32_t frequency_Hz)
{
  
}
