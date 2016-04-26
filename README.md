#STMRGBController 

#Overview

STMRGBController using the accelerometer in the phone to control the LEDs

#Description

Project contain two main parts: application, and microcontroller.
It is connected each other via BlueTooth Master (Phone) - slave (MCU).
Application sends via BT x,y,z positions downloaded from accelerometer, then if MCU receive correct frame with data it sends to RGB LED's according to 3 axes. 
Additional feature is playing LED's by music tone received from application.


#Tools

Aplication: Android Studio 1.5.1, Java
Microcontroller: Atmel Studio 7, C

#How to run
Just connect Your smartphone to BT module and enjoy it!

#How to compile
Quickly without errors

#Future improvements
Awesome RGB effects

#Attributions

http://developer.android.com/guide/topics/connectivity/bluetooth.html

#License

MIT

#Credits

Norbert Hanysz
Mikołaj Owczarczak 

The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology. 

Supervisor: Michał Fularz
