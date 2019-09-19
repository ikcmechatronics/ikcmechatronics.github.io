#include <16F877A.h>
#device ADC=10

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=20000000)

unsigned long int _readAnalog1;
unsigned long int _readAnalog2;

void main()
{
  setup_adc(adc_clock_div_32);
  setup_adc_ports(ALL_ANALOG);


  while(TRUE)
  {
    set_adc_channel(0);
    _readAnalog1 = read_adc();
    delay_ms(1);

    set_adc_channel(1);
    _readAnalog2 = read_adc();
    delay_ms(1);

    delay_ms(100);
  }
}
