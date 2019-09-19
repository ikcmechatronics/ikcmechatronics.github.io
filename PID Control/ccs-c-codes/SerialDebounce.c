#include <16F877A.h>

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES NOBROWNOUT            	//No brownout reset
#FUSES NOLVP                 	//No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=20000000)
#use rs232 (baud=9600,xmit=PIN_C6, rcv=PIN_C7, parity=N, stop=1) 

void main()
{

  printf("PIC16F877A UART example");          // UART Write

  while(TRUE)
  {
    if(kbhit())   // If data has been received
    {                             
      char i = getc();                        // UART read
      putc(i);                                // Send it back
    }
    delay_ms(100);
  }
}
