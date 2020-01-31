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


void loop()
{ 
  static int16_t index = -1;
  int i; 

  Block *block=NULL;
  // grab blocks!
  pixy.ccc.getBlocks();

  if (index==-1) // search....
  {
    index = acquireBlock();
  }
  
  // If we've found a block, find it, track it
  if (index>=0)
     block = trackBlock(index);

  if (Serial.available() > 0) {
    String s = Serial.readStringUntil('\n');
    Serial.println(s);
    Serial.flush();

    
    if (block)
    {
      Serial.print('S');
      Serial.print(block->m_x);
      Serial.println('E');
    } else {
      index = -1; // set search state
      Serial.print('S');
      Serial.print(-1);
      Serial.println('E');
    }  
  }
}
