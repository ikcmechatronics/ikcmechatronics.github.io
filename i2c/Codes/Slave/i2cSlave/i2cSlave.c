#include <16F877A.h>
#device ADC=10

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=16000000)     // using crystal
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)  // Serial communication - can be used for debugging purposes
#use i2c(Slave,sda=PIN_C4,scl=PIN_C3,force_hw,address=0x40)                // i2c slave initialization. hardware pins are used and the address is specified.

#include <stdio.h>
#include <stdlib.h> 

const int bufferLength = 10;              // Character arrays buffer (can be changed up to project requirements)
char dataToSend[bufferLength];            // Sending data from slave to master char array (maximum size depends on the bufferLength variable - can be changed up to project requirements)
char *dataToMaster;                       // Undimensional character array of sending data
char masterDataBuffer[bufferLength];      // Character array of receiving data from master (maxmum size depends on the bufferLength variable - can be changed up to project requirements) 
unsigned long int *masterBuffer;          // Undimensional integer array of receiving data
int previousIndex = 1;                    // Variable to be used in arranging data to send
int8 state;                               // i2c receiving variable

unsigned long int ledPwm;                 // Led Pwm variable
unsigned long int potValue;               // Potentiometer variable

// Main function for receiving and sending i2c packages according to state (i2c commands)
#INT_SSP
void  SSP_isr(void) 
{
   state = i2c_isr_state();
   
   if((state== 0 ) || (state== 0x80))
     i2c_read();
   
   // Master requests data - dataToMaster is updated in the main loop.
   if(state >= 0x80)
   {
      i2c_write(dataToMaster[state - 0x80]);
   }
   // Master writes data to the slave - the elements of masterDataBuffer are used in the slave operations.
   else if(state > 0)
   {
      masterDataBuffer[state - 1] = i2c_read();
   }
}

// This is the function that arranges the sending package on i2c bus.
// This function manipulates the array dataToSend according to parameters of dataToConvert and startIndex
void ArrangeDataToBeSent(unsigned long int dataToConvert, int startIndex)
{
   int digitSize = 0;
   float numerator = (float)dataToConvert;
   if (dataToConvert == 0)
   {
      digitSize = 1;
   }
   else
   {
      while(numerator >= 1)
      {
         numerator = numerator/10.0;
         digitSize++;
      }
   }
   char arrangeData[bufferLength];
   sprintf(arrangeData,"%lu,",dataToConvert);
   for (int i = 0; i < digitSize; i++)
   {
      dataToSend[startIndex+i+1] = arrangeData[i];
   }
   memset(arrangeData,0,digitSize);
   previousIndex = startIndex+i;
}

// This is the function that splits the receiving package with respect to start, split and end characters.
// The function returns an unsigned long int array dynamically.
// Ex: if the receiveBuffer is *101,50# => splittedInt = [101 50] - 2 dimensional unsigned long int array
unsigned long int *SplitReading(char *receiveBuffer, char startChar, char splitChar, char endChar)
{
   //char splitChar = ',';
   int splitIndex[bufferLength];
   unsigned long int splittedInt[bufferLength];
   int splitArrayIndex = 0;
   int lastIndex = 0;
   
   for (int s = 0; s < bufferLength; s++)
   {
      if (receiveBuffer[s] == startChar)
      {
         splitIndex[splitArrayIndex] = s;
         splitArrayIndex++;
      }
      else if (receiveBuffer[s] == splitChar)
      {
         splitIndex[splitArrayIndex] = s;
         splitArrayIndex++;
      }
      else if (receiveBuffer[s] == endChar)
      {
         splitIndex[splitArrayIndex] = s;
         lastIndex = s;
         break;
      }
   }
   for (int b = 0; b < bufferLength-1; b++)
   {
      if (splitIndex[b] == lastIndex)
      {
         break;
      }
      else
      {
         int dim = splitIndex[b+1] - splitIndex[b] - 1;
         char splittingParts[bufferLength];
         int start = splitIndex[b] + 1;
         for (int c = 0; c < dim; c++)
         {
            splittingParts[c] = receiveBuffer[start];
            start++;
         }
         splittedInt[b] = atol(splittingParts);
         memset(splittingParts,0,bufferLength);
      }
   }
   return splittedInt;
}

void main()
{
   setup_adc(adc_clock_div_32);
   setup_adc_ports(AN0);
   
   // PWM Define
   setup_ccp1(CCP_PWM);
   setup_timer_2(T2_DIV_BY_16, 255, 1);
   ledPwm = 0;
   set_pwm1_duty(ledPwm);
   
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   SET_TRIS_D(0);

   while(TRUE)
   {
      set_adc_channel(0);                                // Setting analog input on channel 0
      potValue = read_adc();                             // Reading analog input on channel 0 and assigning it to potValue variable
      delay_ms(1);
      
      ledPwm = potValue*0.249;                           // Mapping the analog value between 0-255
      
      // i2c Write Part => Package => *data1,data2#
      dataToSend[0] = '*';                               // First character to be sent
      ArrangeDataToBeSent(ledPwm, 0);                    // First value to be sent
      dataToSend[previousIndex+1] = ',';                 // Character to be splitted
      ArrangeDataToBeSent(ledPwm, previousIndex+1);      // Second value to be sent
      dataToSend[previousIndex+1] = '#';                 // End character to be sent
      dataToMaster = dataToSend;                         // Updating final value
      printf("%s\n",dataToMaster);                       // Printing to serial communication display for debug purposes
      memset(dataToSend,0,bufferLength);                 // Deleting the variable dataToSend
      
      masterBuffer = SplitReading(masterDataBuffer,'*',',', '#');    // Splitting reading values and convert them to integer array
      set_pwm1_duty(masterBuffer[0]);                    // Setting pwm duty cycle between 0 - 255
      printf("\n");
   }
}

