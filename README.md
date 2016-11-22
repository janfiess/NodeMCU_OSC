Voilà the project files for my instructable on YouTube.

This tutorial shows how to make a low budget NodeMCU wireless IoT controller communicate over OSC. The controller board is using an ESP8266 chip and is coded using Arduino. 
We cover all the steps (hooking up the board, installling the required software, coding) in order to make the controller board
-   receiving OSC messages (eg. to switch an LED on or off per smartphone)
-   sending OSC messages (eg. to get informed via smartphone that a button is pressed).

In order to follow this instructable, you need some hardware and software:

NodeMCU v1.0 (ESP8266 Wifi module with integrated programmer and micro USB connection)

Arduino IDE 1.6.5 and up

Button, LED, Resistors (eg. 220 ohms and 10kohms)

Micro USB cable for programming the NodeMCU

TouchOSC app for your smartphone or tablet (for 4,99 euros: Android: https://play.google.com/store/apps/details?id=net.hexler.touchosc_a&hl=deor or iOS: https://itunes.apple.com/de/app/touchosc/id288120394?mt=8) 

TouchOSC desktop editor for configuring the mobile interfaces: http://hexler.net/software/touchosc
On current Mac OS operating systems (now is 2016 - Java 8), you may have trouble running the app because it was developed with Java 6. To get such apps running install https://support.apple.com/kb/DL1572?viewlocale=en_US&locale=en_US

Wifi network with SSID and password

ESP8266 library for Arduino: http://arduino.esp8266.com/stable/package_esp8266com_index.json

OSCuino library for Arduino: https://github.com/CNMAT/OSC
