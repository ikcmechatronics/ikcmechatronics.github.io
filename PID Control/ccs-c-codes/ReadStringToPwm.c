#include <16F877A.h>
#include <stdlib.h>

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES NOBROWNOUT            	//No brownout reset
#FUSES NOLVP                 	//No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=20000000)
#use rs232 (baud=9600,xmit=PIN_C6, rcv=PIN_C7, parity=N, stop=1) 

int counter = 0;
char strInput[4];
unsigned long pwm;

void main()
{
   while(TRUE)
   {
      if(kbhit())   // If data has been received
    {                             
      char i = getc();                        // UART read
      if (i == '*')
      {
         counter = 0;
         printf(strInput);
         pwm = atol(strInput);
         printf("sayi %lu",pwm);
         memset(strInput, 0, 4);
      }
      else
      {
         strInput[counter] = i;
         counter++;
      }
    }
   }
}
