[Information about i2c Communication](https://github.com/ikcmechatronics/ikcmechatronics.github.io/blob/master/i2cInfo.md)

## Introduction

This is an information page about i2c communication between devices. 

In the purpose of testing the i2c communication, in this page [Arduino Uno master](https://github.com/ikcmechatronics/ikcmechatronics.github.io/blob/master/i2c/Codes/Master/i2cMaster/i2cMaster.ino) and [PIC16f877a slave](https://github.com/ikcmechatronics/ikcmechatronics.github.io/blob/master/i2c/Codes/Slave/i2cSlave/i2cSlave.c) functions are provided. You can easily understand and apply them to your project with some modifications according to your software requirements.

** FOR RASPBERRY PI USERS: Python code for [Raspberry PI as i2c master](https://github.com/ikcmechatronics/ikcmechatronics.github.io/blob/master/i2c/Codes/Master/i2cMaster/i2cRaspberryMaster.py) is uploaded. Here, it should be noted that smbus2 library is used, please first [download](https://github.com/kplindegaard/smbus2) and install this library to your single board computers. By using this code, you can read and write multiple data via i2c protocol with devices that supports pure i2c (Arduino, PIC etc). The following challenges are solved by using the code with smbus2 library.

The challenge of using raspbery pi to communicate via i2c is that the python uses the smbus protocol, which uses different data package in multiple data transmission than pure i2c. More information about conflict in between smbus and i2c protocol in multiple data transfer can be seen below;

![](/i2c/i2c_smd_multiple_byte.PNG)

However, if the data transfer is not multiple (single byte read or write), there is no conflict between protocols, thus the modification is not needed in software. More information about data package in reading and writing single byte can be seen below;

![](/i2c/i2c_smd_single_byte.PNG)

## Real Time Demo Experience
Project Groups have opportunity to enroll for demonstration of i2c system in the laboratory. Also, it is possible for gorups to use i2c test board for debugging purposes with appointment. For more information about i2c test board, please click [here](https://github.com/ikcmechatronics/ikcmechatronics.github.io/blob/master/i2c/i2c%20Hardware%20Board/i2c%20Test%20Board%20Information%20Document.pdf) to see the document.

Project groups should arrange an appointment with sending a message to mertcan.kocak@ikc.edu.tr. At least 51% of group members must attend to the demonstration. (If project group consists of 4 students, 3 students must attend).

## Notes and recommendations about i2c communication

* If you prefer CCS C Compiler as the compiler, it is recommended to use the version 5.008 or later.

* Start simple code parts to achieve the communication. Then implement complex algorithms.

* It is better to start with scanning the devices on the bus from master. For this purpose, first upload simple slave programs to slave microcontrollers with predefined addresses. Then, i2c scanner codes can be used to understand whether there is a hardware problem or not. Also, with this procedure, the possiblity to be disorder by means of addresses vanishes. You can reach the i2c scanner code for Arduino Uno as a master device from [here.](https://github.com/ikcmechatronics/ikcmechatronics.github.io/blob/master/i2c/Codes/Master/i2cScanner/i2cScanner.ino)

*This page will be updated with the new works. Also, it is possible for you to contribute us to develop a better algorithms by means of i2c communication.

Additional document about Fuzzy Control with Matlab is [here.](https://github.com/ikcmechatronics/ikcmechatronics.github.io/blob/master/i2c/MEE406%20Fuzzy%20Control%20Demo.pdf)
