#include <16F877A.h>

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES NOBROWNOUT            	//No brownout reset
#FUSES NOLVP                 	//No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=20000000)

float pwmOut;

void main()
{
  setup_ccp1(CCP_PWM);
  setup_timer_2(T2_DIV_BY_16, 255, 1);
  pwmOut = 50;

  while(TRUE)
  {
    set_pwm1_duty(pwmOut);
    delay_ms(100);
  }
}
