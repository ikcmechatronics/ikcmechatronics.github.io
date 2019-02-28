#include <Wire.h>                 // Including i2c library

int slave1PotValue = 0;           // Slave device potentiometer integer variable
int masterPot = 0;                // Master device potentiometer 1 variable
int masterPot2 = 0;               // Master device potentiometer 2 variable
int masterPot3 = 0;               // Master device potentiometer 3 variable
int masterPot4 = 0;               // Master device potentiometer 4 variable
const int slaveArrayLength = 2;   // Could be changed to another value according to slave values

void setup()
{
  Wire.begin();                   // Join i2c bus as master
  Serial.begin(9600);             // Start serial communication
  pinMode(3,OUTPUT);              // LED is connected to pin 3 as pwm output.
}

void loop()
{
  masterPot = analogRead(A0);                   // Reading analog value of the master device.
  masterPot = map(masterPot,0,1023,0,255);      // Mapping the analog value between 0-255
  masterPot2 = analogRead(A1);                  // Reading analog value of the master device.
  masterPot2 = map(masterPot2,0,1023,0,255);    // Mapping the analog value between 0-255
  masterPot3 = analogRead(A2);                  // Reading analog value of the master device.
  masterPot3 = map(masterPot3,0,1023,0,255);    // Mapping the analog value between 0-255
  masterPot4 = analogRead(A3);                  // Reading analog value of the master device.
  masterPot4 = map(masterPot4,0,1023,0,255);    // Mapping the analog value between 0-255
  
  int digitSize = DigitSize(masterPot);         // Figuring out the digit size of the analog read value in order to be used in the character array dimension.
  int digitSize2 = DigitSize(masterPot2);       // Figuring out the digit size of the analog read value in order to be used in the character array dimension.
  int digitSize3 = DigitSize(masterPot3);       // Figuring out the digit size of the analog read value in order to be used in the character array dimension.
  int digitSize4 = DigitSize(masterPot4);       // Figuring out the digit size of the analog read value in order to be used in the character array dimension.
  
  char cstr[digitSize];                         // Creating the character array of the i2c bus package.
  char cstr2[digitSize2];                       // Creating the character array of the i2c bus package.
  char cstr3[digitSize3];                       // Creating the character array of the i2c bus package.
  char cstr4[digitSize4];                       // Creating the character array of the i2c bus package.
  itoa(masterPot, cstr, 10);                    // Filling the cstr array with the characters of the master pot. Ex: if masterPot = 15, then cstr[0] = '1' , cstr[1] = '5'
  itoa(masterPot2, cstr2, 10);                  // Filling the cstr array with the characters of the master pot2. Ex: if masterPot = 15, then cstr[0] = '1' , cstr[1] = '5'
  itoa(masterPot3, cstr3, 10);                  // Filling the cstr array with the characters of the master pot3. Ex: if masterPot = 15, then cstr[0] = '1' , cstr[1] = '5'
  itoa(masterPot4, cstr4, 10);                  // Filling the cstr array with the characters of the master pot4. Ex: if masterPot = 15, then cstr[0] = '1' , cstr[1] = '5'
  
  // i2c Write to address 0x18 => Package => *data1,data2#
  Wire.beginTransmission(0x18);                 // Begining transmission between slave with the address of 0x18
  Wire.write('*');                              // Sending the character of '*'    (initial character of package)
  i2cWrite(cstr, digitSize);                    // Sending the cstr array => masterPot value
  Wire.write(',');                              // Sending the character of ','    (splitting character of package)
  i2cWrite(cstr2, digitSize2);                  // Sending the cstr array => masterPot2 value
  Wire.write('#');                              // Sending the character of '#'    (final character of package)
  Wire.endTransmission();                       // Stopping transmission between slave with the address of 0x18

  // i2c Write to address 0x20 => Package => *data1,data2#
  Wire.beginTransmission(0x20);                 // Begining transmission between slave with the address of 0x20
  Wire.write('*');                              // Sending the character of '*'    (initial character of package)
  i2cWrite(cstr3, digitSize3);                  // Sending the cstr array => masterPot3 value
  Wire.write(',');                              // Sending the character of ','    (splitting character of package)
  i2cWrite(cstr4, digitSize4);                  // Sending the cstr array => masterPot4 value
  Wire.write('#');                              // Sending the character of '#'    (final character of package)
  Wire.endTransmission();                       // Stopping transmission between slave with the address of 0x20

  String receiveBuffer = i2cRead(0x18, 10);     // Reading 10 bytes long from slave address 0x18 
  int* receiveArray;                            // Integer array without length
  receiveArray = SplitReading(receiveBuffer, ',', '#');   // Splitting reading values and convert them to integer array
  
  slave1PotValue = receiveArray[0];             // Assigning first reading of slave package. Ex: if receive string is *101,50# => slave1PotValue = 101; (receiveArray[0])
  analogWrite(3,slave1PotValue);                // Pwm output from pin 3 with the value of slave1PotValue
  delay(50);
}

// This is the function that gets the character array and its actual digit size as parameters and write all the information to the i2c bus.
// This function should be used with Wire.beginTransmission() and Wire.endTransmission();
void i2cWrite(char *charArray, int _digitSize)
{
  for (int i = 0; i < _digitSize; i++)
  {
    Wire.write(charArray[i]);
  }
}

// This is the function that reads the data of the specified address. Reading length is also specified with readLength variable.
String i2cRead(uint8_t address, int readLength)
{
  String _receiveBuffer = "";
  Wire.requestFrom(address, readLength); 
  while(Wire.available())
  { 
    char c = Wire.read();
    _receiveBuffer += c;
  }
  return _receiveBuffer;
}

// This is the function that convert reading data to integer array.
// Integer array length is based on the splitting elements.
// For example; if the comming value at specified time is *101,99# then the function returns the integer array of [101 99] (the length is 2).
int *SplitReading(String _receiveBuffer, char _splitChar, char _endChar)
{
  int splitCharIndex = 0;
  int prevSplitCharIndex = 0;
  int lastSplitCharIndex = _receiveBuffer.lastIndexOf(_splitChar);
  int stringEndIndex = _receiveBuffer.indexOf(_endChar);
  static int splittedInt[slaveArrayLength];
  int c = 0;
  while (splitCharIndex < lastSplitCharIndex)
  {
    splitCharIndex = _receiveBuffer.indexOf(_splitChar, prevSplitCharIndex + 1);
    splittedInt[c] = (_receiveBuffer.substring(prevSplitCharIndex + 1,splitCharIndex)).toInt();
    prevSplitCharIndex = splitCharIndex;
    c++;
    if (prevSplitCharIndex == lastSplitCharIndex)
    {
      splittedInt[c] = (_receiveBuffer.substring(prevSplitCharIndex + 1,stringEndIndex)).toInt();
      break;
    }
  }
  return splittedInt;
}

// This is the function that returns the digit size of the input integer data.
// For example; if the data is 99 then the function returns 2. If the data is 0 then the function returns 1.
int DigitSize(int data)
{
  int digitSize = 0;
  float numerator = (float)data;
  if (data == 0)
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
  return digitSize;
}
