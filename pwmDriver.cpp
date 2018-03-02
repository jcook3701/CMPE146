#include "pwmDriver.hpp"

//bit 6 in LPCSRC->PCONP turns on/off the pwm
static const uint8_t pwm_pin = 6; 

void PWMDriver::pwmSelectAllPins()
{
  LPC_SC->PCONP |= (0x1 << pwm_pin); 
  //PWM_PIN_2_0
  LPC_PINCON->PINSEL4 &= ~(0x3 << 0);
  LPC_PINCON->PINSEL4 |=  (0x2 << 0); 
  //PWM_PIN_2_1
  LPC_PINCON->PINSEL4 &= ~(0x3 << 2);
  LPC_PINCON->PINSEL4 |=  (0x2 << 2); 
  //PWM_PIN_2_2
  LPC_PINCON->PINSEL4 &= ~(0x3 << 4);
  LPC_PINCON->PINSEL4 |=  (0x2 << 4);
  //PWM_PIN_2_3
  LPC_PINCON->PINSEL4 &= ~(0x3 << 6);
  LPC_PINCON->PINSEL4 |=  (0x2 << 6);
  //PWM_PIN_2_4
  LPC_PINCON->PINSEL4 &= ~(0x3 << 8);
  LPC_PINCON->PINSEL4 |=  (0x2 << 8); 
  //PWM_PIN_2_5
  LPC_PINCON->PINSEL4 &= ~(0x3 << 10);
  LPC_PINCON->PINSEL4 |=  (0x2 << 10);
}

void PWMDriver::pwmSelectPin(PWMDriver::PWM_PIN pwm_pin_arg)
{
  LPC_SC->PCONP |= (0x1 << pwm_pin);   
  switch(pwm_pin_arg)
    {
    case PWM_PIN_2_0:
      LPC_PINCON->PINSEL4 &= ~(0x3 << 0);
      LPC_PINCON->PINSEL4 |=  (0x2 << 0); 
      break;
    case PWM_PIN_2_1:
      LPC_PINCON->PINSEL4 &= ~(0x3 << 2);
      LPC_PINCON->PINSEL4 |=  (0x2 << 2); 
      break;
    case PWM_PIN_2_2:
      LPC_PINCON->PINSEL4 &= ~(0x3 << 4);
      LPC_PINCON->PINSEL4 |=  (0x2 << 4); 
      break;
    case PWM_PIN_2_3:
      LPC_PINCON->PINSEL4 &= ~(0x3 << 6);
      LPC_PINCON->PINSEL4 |=  (0x2 << 6); 
      break;
    case PWM_PIN_2_4:
      LPC_PINCON->PINSEL4 &= ~(0x3 << 8);
      LPC_PINCON->PINSEL4 |=  (0x2 << 8); 
      break;
    case PWM_PIN_2_5:
      LPC_PINCON->PINSEL4 &= ~(0x3 << 10);
      LPC_PINCON->PINSEL4 |=  (0x2 << 10); 
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
