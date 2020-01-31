//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//
// This sketch is a good place to start if you're just getting started with 
// Pixy and Arduino.  This program simply prints the detected object blocks 
// (including color codes) through the serial console.  It uses the Arduino's 
// ICSP SPI port.  For more information go here:
//
// https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:hooking_up_pixy_to_a_microcontroller_-28like_an_arduino-29
//
  
#include <Pixy2.h>

// This is the main Pixy object 
Pixy2 pixy;

void setup()
{
  Serial.begin(115200);
  
  pixy.init();
  pixy.setLamp(1,1);


}

// Take the biggest block (blocks[0]) that's been around for at least 30 frames (1/2 second)
// and return its index, otherwise return -1
int16_t acquireBlock()
{
  if (pixy.ccc.numBlocks && pixy.ccc.blocks[0].m_age>30)
    return pixy.ccc.blocks[0].m_index;
  return -1;
}

// Find the block with the given index.  In other words, find the same object in the current
// frame -- not the biggest object, but he object we've locked onto in acquireBlock()
// If it's not in the current frame, return NULL
Block *trackBlock(uint8_t index)
{
  uint8_t i;

  for (i=0; i<pixy.ccc.numBlocks; i++)
  {
    if (index==pixy.ccc.blocks[i].m_index)
      return &pixy.ccc.blocks[i];
  }

  return NULL;
}

void sendData(int x, int y) {
  // Check to see if we have any data waiting to be read.  This is our cue to send our data if we
  //   have any.  
  static char bByte;
  static char tmpString[12];          // Used to store the tracking information we are sending over serial.

  
  if (Serial.available()) {
    while (Serial.available() != 0){
      bByte = Serial.read();
      if ((bByte == 'T') || (bByte == 't')) {
        pixy.setLamp(1,1);
      }
      if ((bByte == 'F') || (bByte == 'f')) {
        pixy.setLamp(0,0);
      }
    }
    
    // If we currently have a block acquired, send it's lateral position and height.  The values that are passed to us 
    // are between 0 and 319.  We want an offset from the middle of the screen, so we subtract 160 (half of 320) 
    // from the position to get values between -160 and 160.  The Height of the block is sent as well so that we
    // can estimate distance to the object.
    if (x != -999) {      
      sprintf(tmpString, "S%04d,%03dE", x - 160, y);
    } else {
      sprintf(tmpString, "S%04dE", -999);
    }  
    Serial.println(tmpString);      
  }
}


void loop()
{ 
  static int16_t index = -1;  // The index of the block we are tracking.  -1 if we are not tracking a block.

  // *block holds a pointer to an individual block.  Initially set to NULL
  Block *block=NULL;
  
  // Ask the pixy to scan for an array of blocks.
  pixy.ccc.getBlocks();

  if (index==-1) // If we don't have a block that we are interested in, see if we can find on.
  {
    index = acquireBlock();
  }
  
  // If we've found a block, find it, track it
  if (index>=0)
     block = trackBlock(index);

  if (!block) {
    index = -1;
    sendData(-999,0);
  } else {
    sendData(block->m_x, block->m_height);
  }
}
