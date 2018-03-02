#include "pwmDriver.hpp"

//bit 6 in LPCSRC->PCONP turns on/off the pwm
static const uint8_t pwm_pin = 6; 

static const uint8_t pclk_pwm_pim = 12; 

void PWMDriver::PWMDriverInit(){
  LPC_SC->PCONP |= (0x1 << pwm_pin);  //PCPWM1 - PWM1 power/clock control bit.
  LPC_SC->PCLKSEL0 |= (0b01 << pclk_pwm_pim);  //PCLK_PWM1 - PCLK_peripheral = CCLK
  LPC_PWM1->MR0;

}

void PWMDriver::pwmSelectAllPins()
{
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
  LPC_PWM1->PCR |= (0b111111 << 9); //0x3F - Enable PWM Outputs
  LPC_PWM1->PCR |= (0b00000 << 2);  //0x00 - Single Edged
  LPC_PWM1->PR  |= PWMPRESCALE;     //1 micro-second resolution
  
}

void PWMDriver::setDutyCycle(PWMDriver::PWM_PIN pwm_pin_arg, float duty_cycle_percentage)
{

  
  pwmSelectPin(pwm_pin_arg);
  switch(pwm_pin_arg)
    {
    case PWM_PIN_2_0:
      
      LPC_PWM1->LER |= (1 << 0); //update value in MR0
      break;
    case PWM_PIN_2_1:

      LPC_PWM1->LER |= (1 << 1); //update value in MR1
      break;
    case PWM_PIN_2_2:

      LPC_PWM1->LER |= (1 << 2); //update value in MR2
      break;
    case PWM_PIN_2_3:

      LPC_PWM1->LER |= (1 << 3);  //update value in MR3 
      break;
    case PWM_PIN_2_4:

      LPC_PWM1->LER |= (1 << 4);  //update value in MR4
      break;
    case PWM_PIN_2_5:

      LPC_PWM1->LER |= (1 << 5);  //update value in MR5
      break;
    }
  
}

void PWMDriver::setFrequency(uint32_t frequency_Hz)
{
  
}
