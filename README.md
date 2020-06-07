MCard1802Terminal
================
Simple Arduino based Terminal program for the 1802 Membership Card Starter Kit.

Introduction
-------------
This repository contains an Arduino Serial Terminal program for use with the 1802 Membership Card Starter Kit.

The 1802 Membership Card Starter Kit is available from Lee Hart on his [website.](http://www.sunrise-ev.com/1802.htm)  
Additional documentation and other information are available from Herb Johnson's 
[1802 Membership Card website.](http://www.retrotechnology.com/memship/memship.html)

This program can be used to communicate with the MCSMP20A Super Monitor program running on the ROM supplied with
the Membership Card Starter Kit.


This code uses the AltSoftSerial library by Paul Stoffregen for Serial communication to the 1802 Membership Card. 
This library is available through the Arduino Library Manager or in the [PaulStoffregen/AltSoftSerial repository](https://github.com/PaulStoffregen/AltSoftSerial)
on GitHub. For Inverse Logic an updated version of the AltSoftSerial library is available in the [fourstix/AltSoftSerial repository](https://github.com/fourstix/AltSoftSerial)
forked from the master repository on GitHub.  [Pull Request #59](https://github.com/PaulStoffregen/AltSoftSerial/pull/59) will merge this code update into the master.

For more programs supporting the 1802 Membership Card with various Arduino based hardware please see the 
[fourstix/MCard1802Arduino repository](https://github.com/fourstix/MCard1802Arduino) on GitHub.


QuickStart Instructions
-----------------------
 Connect the Arduino to the Membership Card as follows:
 
 Arduino Pin     MCard1802
   9 (TX)        /EF3 (RX)   P1 - 26 
   8 (RX)          Q  (TX)   P1 - 12
   GND             GND       P1 -  1 (and/or P1 - 30)
   +5V		   VDD	     P1 - 14 (Optional, you can power Membership card with its own 5v supply)
   
 (Note: if you power the Membership Card through the Arduino +5V pin, 
 supply external power to the Arduino power jack since USB power alone
 is not quite enough to reliably power both the Arduino and Membership Card together.)

 Start the Monitor program on the 1802 Membership Card,
 Then run this sketch on the Arduino.  (Note you may need to 
 connect pins P1-13 and P1-14, Run and Vdd, and also connect 
 pins P1-10 and P1-11, /MWR and /WE, for the Membership card 
 to run correctly. The Monitor program is usually in the ROM 
 chip supplied, labeled something like MCSMP20A.)
  
 Start the Arduino Serial Monitor in the Arduino IDE, using 
 the Menu Tools > Serial Monitor or the Shift+Ctrl+M hot key.
 Set the baud rate to 9600 and the Line Ending to Carriage return.
 
 Text typed in the Arduino Serial Monitor will be sent to the
 1802 Membership Card and the monitor will display the response.
 
Installation
------------

* Connect the power to the Membership Card Starter Kit according to the instructions.  
   * P1-14 VDD is connected to +5V (If using Arduino +5V for VDD, use external power to Arduino)
   * P1-1 and P1-30 GND should be connected to ground.
   * P1-13 and P1-14 (RUN and VDD) should be connected together.
   * P1-10 and P1-11 (/WE and /MWR) should be connected together. 
   
* Connect the Arduino Serial pins to the 1802 Membership Card Serial pins.
   * Connect Arduino Pin 8 (RX) to Q P1-12 on the Membership Card
   * Connect Arduino Pin 9 (TX) to /EF3 P1-26 on the Membership Card.
   * Connect Arduino GND to GND, P1-1 and P1-30, on the Membership Card.
   * If desired, connect Arduino +5V to VDD P1-14 (and use external power to Arduino for reliable operation.)
   
* Install the AltSoftSerial library using the Arduino Library Manager
   * In the Menu of the Arduino IDE, go to Tools > Manage Libraries...
   * In the Library Manager window, type in "AltSoftSerial"
   * In the listing for "AltSoftSerial by Paul Stoffregen", click on the Install button.
   
* Load the code to Arduino from your computer using the USB cable.
   * Download and open the MCard1802Terminal.ino file in the Arduino IDE
   * Connect the Arduino to your computer with the USB cable
   * Select the proper board and port for your Arduino
   * Click on the Upload button to compile and run the code on your Arduino
   
* Open the Serial Monitor 
   * In the Arduino IDE select Tools > Serial Monitor (or use hotkey Ctrl-Shift-M)
   * Set the Baud rate to 9600
   * Set the Line Ending to "Carriage return"   
   * You should see the Terminal Menu followed by the Monitor program's initial response.
   * If needed, press enter to send a blank line with a Carriage return
   * If no response, disconnect Run P1-13 from VDD P1-14, briefly, then reconnect and try again.  
   * Alternatively, you can use a terminal program like RealTerm instead of the Arduino IDE Serial Monitor.
   * Please see this [Sparkfun Terminal Tutorial](https://learn.sparkfun.com/tutorials/terminal-basics/real-term-windows) for more information on RealTerm.
   
* For Inverse Logic   
   * To run an 1802 Membership card with a ROM with inverse logic like MCSMP20J.
   * Invert the output of Q with a transistor or logic inverter to create /Q (NOT Q or Inverse Q).
   * Connect /Q (NOT Q) to Arduino pin 8 (RX), connect pin 9 (TX) to /EF3 P1-26 (The software will invert TX.)
   * Connect Arduino GND to GND, P1-1 and P1-30, on the Membership Card.
   * Obtain a version of AltSoftSerial that supports invese logic from [fourstix/AltSoftSerial](https://github.com/fourstix/AltSoftSerial)
   * See [Pull Request #59](https://github.com/PaulStoffregen/AltSoftSerial/pull/59) for status of code changes in main repository. Once merged, this function will be available in the main repository.
   * Manually install this Arduino Library. Please see this [Sparkfun Arduino Library Tutorial](https://learn.sparkfun.com/tutorials/installing-an-arduino-library) for details.
   * Compile the Arduino code with AltSoftSerial(true) contstructor.
   * Open the Serial Monitor as before. For programs with ANSI graphics, like in Adventureland, use a terminal program like [RealTerm.](https://realterm.sourceforge.io/)
   * Please see this [Sparkfun Terminal Tutorial](https://learn.sparkfun.com/tutorials/terminal-basics/real-term-windows) for more information on RealTerm.
   
Examples
--------
<table class="table table-hover table-striped table-bordered">
  <tr align="center">
   <td><img src="https://github.com/fourstix/MCard1802Terminal/blob/master/pics/ArduinoConnection.jpg"></td>
   <td><img src="https://github.com/fourstix/MCard1802Terminal/blob/master/pics/ConnectionsFritzing.jpg"></td> 
   <td><img src="https://github.com/fourstix/MCard1802Terminal/blob/master/pics/TerminalWindow.jpg"></td> 
  </tr>
  <tr align="center">
    <td>Picture of Arduino connected to the 1802 Membership Card</td>
    <td>Fritzing diagram showing connections</td>
    <td>Screen shot of Terminal Window with the 1802 Membership Card Monitor program's initial response.</td>
  </tr>
  <tr align="center">
    <td><img src="https://github.com/fourstix/MCard1802Terminal/blob/master/pics/RealTerm1.jpg"></td>
    <td><img src="https://github.com/fourstix/MCard1802Terminal/blob/master/pics/RealTerm2.jpg"></td> 
  </tr>
  <tr align="center">
    <td>Screen shot of RealTerm Window with the 1802 Membership Card running MCSMP20J with Inverse Serial Logic.</td>
    <td>Screen shot of RealTerm Window with the 1802 Membership Card running MCSMP20J with Adventureland ANSI graphics.</td>        
  </tr>
 </table>
 
Repository Contents
-------------------
* **/src/MCard1802Terminal/**
  * MCard1802Terminal.ino -- Arduino based Serial Terminal for the 1802 Membership Card Starter Kit.
* **/pics/** -- example pictures used in README.md


License Information
-------------------

This code is public domain under the MIT License, but please buy me a beer
if you use this and we meet someday (Beerware).

References to any products, programs or services do not imply
that they will be available in all countries in which their respective owner operates.

The AltSoftSerial library by Paul Stoffregen
Copyright (c) 2012 PJRC.COM, LLC, Paul Stoffregen

The 1802 Membership Card Microcomputer 
Copyright (c) 2006-2020  by Lee A. Hart.
 
Many thanks to the original authors for making their designs and code available as open source.

This code, firmware, and software is released under the [MIT License](http://opensource.org/licenses/MIT).

The MIT License (MIT)

Copyright (c) 2020 by Gaston Williams

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

**THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.**