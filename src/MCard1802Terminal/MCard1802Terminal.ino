/*
 * Serial Terminal Testing with 1802 Membership Card
 * 
 * Copyright (c) 2020 by Gaston Williams
 * 
 * Based on the 1802 Membership card hardware by Lee Hart. 
 * The 1802 Membership card is available here: 
 * http://www.sunrise-ev.com/1802.htm 
 * 
 * The 1802 Membership Card Microcomputer 
 * Copyright (c) 2006-2020  by Lee A. Hart.
 * 
 * This code uses the AltSoftSerial library by Paul Stoffregen
 * Copyright (c) 2012 PJRC.COM, LLC, Paul Stoffregen, paul@pjrc.com
 * 
 * QuickStart Instructions:
 * 
 * Connect the Arduino to the Membership Card as follows:
 * 
 * Arduino Pin     MCard1802
 *   9 (TX)        /EF3 (RX)   P1 - 26
 *   8 (RX)          Q  (TX)   P1 - 12 (Positive logic)
 *   GND             GND       P1 -  1 (or P1 - 30)
 *
 * Start the Monitor program on the 1802 Membership Card,
 * Then run this sketch on the Arduino.  (Note you may need to 
 * connect pins P1-13 and P1-14, Run and Vdd, and also connect 
 * pins P1-10 and P1-11, /MWR and /WE, for the Membership card 
 * to run correctly. The Monitor program is usually in the ROM 
 * chip supplied, labled something like MCSMP20A.)
 * 
 * Start the Arduino Serial Monitor in the Arduino IDE, using 
 * the Menu Tools > Serial Monitor or the Shift+Ctrl+M hot key.
 * Set the baud rate to 9600 and the Line Ending to Carriage return.
 * 
 * Text typed in the Arduino Serial Monitor will be sent to the
 * 1802 Membership Card and the monitor will display the response.
 */

//SoftwareSerial library is *NOT* Full-duplex and won't work here.
#include <AltSoftSerial.h>
//Inverse logic support for AltSoftSerial is avaliable at this fork:
//Pull Request #59: https://github.com/fourstix/AltSoftSerial  

//Mask for 7-bit ASCII
#define ASCII_MASK    0x7F

// AltSoftSerial always uses these pins:
//
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Teensy 3.0 & 3.1  21        20         22
// Teensy 2.0         9        10       (none)
// Teensy++ 2.0      25         4       26, 27
// Arduino Uno        9         8         10
// Arduino Leonardo   5        13       (none)
// Arduino Mega      46        48       44, 45
// Wiring-S           5         6          4
// Sanguino          13        14         12


//Default is with RX pin 8, TX pin 9, No inversion
AltSoftSerial altSerial;

//Inverse Logic: RX pin 8 connected to /Q, hardware inversion
//TX pin 9 connected to /EF3, software does the inversion
//AltSoftSerial altSerial(true);

/*
 * If you are using inverted logic ROM like MCSMP20J 
 * then you need to invert Q with a transistor or logic 
 * inverter and use an updated version of AltSoftSerial 
 * that supports inverse logic through the inverse argument 
 * passed in the constructor AltSoftSerial(true).  The
 * updated AltSoftSerial library will hand the inversion on 
 * the TX line.  This way one can run the latest version 
 * of the 1802 Membership Card Monitor ROM MCSMP20J, that
 * includes Basic3 and Adventureland.
 * 
 * I forked the AltSoftSerial library and added the code 
 * changes to add this function to the AltSoftSerial library. 
 * Pull Request #59 contains changes to support this logic.
 * 
 * Until Pull Request #59 is merged with the main repository.
 * You may get the updated code from this fork:
 * 
 * https://github.com/fourstix/AltSoftSerial
 * 
 * The inverse flag works the same way as it does in 
 * SoftwareSerial library.  The code is identical to the main
 * repository, except for the changes to support the inverse
 * function.
 * 
 */

//Variables for Ascii terminal
boolean escape        = false;
boolean hex_escape    = false;
char    hex_char      = 0x00;

//Setup code runs once
void setup() {
  Serial.begin(9600);

  //2400 baud works well with a 1.8MHz or 2MHz clock
  //Baud rates of 2400, 4800 or 9600 may work with faster clocks
  //A baud rate of 300 may be used for slower clocks  
  altSerial.begin(1200);

 
 
    
  Serial.println("Ready!");
  showTerminalMenu();

  //Establish communication to Membership Card monitor with <CR>
  sendChar('\r');      
}

//run repeatedly sending and receiving characters
void loop() {
    if (Serial.available()) {      
      // If anything comes in Serial (USB),
      // get the new byte and process it
      char nextChar = (char)Serial.read();            
      processTerminalChar(nextChar);     
    }
    if (altSerial.available()) {
      // If anything comes in from the Membership Card
      // get the new byte:
      char inChar = (char) altSerial.read();
  
      //Mask off high bit and show it on the Arduino Monitor
      inChar = (inChar & 0x7F);
  
     Serial.write(inChar);
    } // if altSerial.available
} // loop

//Process characters while in terminal mode
void processTerminalChar(char inChar) {
  //First check to see if this is the second hex digit in \hh escape
  if (hex_escape) {
    processHexEscape(inChar);
    //Hex Escape Sequnce done, reset variables
    hex_escape = false;
    hex_char = 0x00;
  } else if (escape) {  //Next check to see of char in escape sequence    
    processEscape(inChar); 
    //Finished process escape (hex escape flag may still be true);  
    escape = false;
  } else if (inChar == '\\') {  //Next character determines escape sequence
    escape = true;
  } else {  // Send it out to the Membership Card
    sendChar(inChar);   
  }//if-else
} // processTerminalChar

//Process characters in escape sequence
void processEscape(char e_Char) {      
    switch(e_Char) {
      //Backslash
      case '\\':
        sendChar('\\');   
      break;
      
      //Show menu
      case '?':      
        showTerminalMenu();
      break;
      
      //New Line
      case 'n':
      case 'N':
        sendChar('\n');   
      break;
            
      //Carriage Return
      case 'r':
      case 'R':
        sendChar('\r');   
      break;
      
      //Tab
      case 't':
      case 'T':
        sendChar('\t');   
      break;
      
      //Escape
      case 'e':
      case 'E':
        sendChar(0x1b);   
      break;
              
      default: 
         //Hexadecimal escape sequence \h or \hh
         if (isHexadecimalDigit(e_Char)) {
           //Hex escape sequence can be one or two hex characters
           hex_escape = true;
           //Set character to first hex digit
           hex_char = getHexValue(e_Char);           
         } else {
          //Unknown character after escape,
          //send backslash plus character as literal
          sendChar('\\');
          sendChar(e_Char);  
         } //if-else isHexadecimalDigit
      break;      
    } // switch
}
//process characters in hex escape sequence
void processHexEscape(char h_char) {
  if (isHexadecimalDigit(h_char)) {
      //Shift first hex digit into high nibble
      hex_char = hex_char << 4;     
      
      //Put second character into low nibble
      hex_char |= getHexValue(h_char);
      
      //Mask off 8th bit, to ensure valid ASCII and send
      hex_char &= ASCII_MASK;
       
      sendChar(hex_char);
    } else {
      //A Non-hex character terminates the hex escape \h
      //First, Send the hex code we have so far
      sendChar(hex_char);
      
      //Check to see if next char starts another escape sequence
      //Otherwise send the non-hex character
      if (h_char == '\\') {
        escape = true;
      } else {
        sendChar(h_char);
      } // if-else h_Char is backslash      
    } //if=else isHexadecimalDigit
}

//Show the ASCII Terminal menu
void showTerminalMenu() {
  Serial.println();
  Serial.println(F("**** ASCII Terminal ****"));
  Serial.println(F("Note: Set Line Ending to \'Carriage return\'."));
  Serial.println(F("Escape characters:"));  
  Serial.println(F("\\t    tab (09)"));
  Serial.println(F("\\r    carriage return (0D)"));
  Serial.println(F("\\n    new line (0A)"));
  Serial.println(F("\\e    escape (1B), cancel or break command"));
  Serial.println(F("\\\\    backslash (5C)"));
  Serial.println(F("\\hh   hexadecimal ASCII character, 00 to 7F"));
  Serial.println(F("\\?    print this menu"));
  Serial.println();
}


//Get the numeric value of a hexadecimal character
byte getHexValue(char h) {
  byte value = 0x00;
  // check to see what range value is in
  if (isDigit(h)) {
    value = h - '0';   
  } else {
      value = 10 + toupper(h) - 'A';
  } // if else
  return value;
} // getHexValue

//Send a character to the Membership Card
void sendChar(char c_send) {
  //Send the character out on pins 8 and 9
  altSerial.write(c_send);   
  //wait a bit for 1802 to process character sent
  delay(20);  
}
