
/*
Sprite demo.
Animate all 64 hardware sprites.
*/

#include <stdlib.h>
#include <string.h>

// include NESLIB header
#include "neslib.h"

// link the pattern table into CHR ROM
//#link "chr_generic.s"

/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
  0x03,			// screen color

  0x11,0x21,0x27,0x00,	// background palette 0
  0x1C,0x20,0x2C,0x00,	// background palette 1
  0x00,0x10,0x20,0x00,	// background palette 2
  0x06,0x16,0x26,0x00,   // background palette 3

  0x0D,0x16,0x15,0x00,	// sprite palette 0
  0x00,0x37,0x25,0x00,	// sprite palette 1
  0x0D,0x2D,0x3A,0x00,	// sprite palette 2
  0x0D,0x27,0x2A	// sprite palette 3
};

// setup PPU and tables
void setup_graphics() {
  // clear sprites
  oam_clear();
  // set palette colors
  pal_all(PALETTE);
  // turn on PPU
  ppu_on_all();
}

// number of actors
#define NUM_ACTORS 16		// 64 sprites (maximum)

// actor x/y positions
byte actor_x[NUM_ACTORS];	// horizontal coordinates
byte actor_y[NUM_ACTORS];	// vertical coordinates

// actor x/y deltas per frame (signed)
sbyte actor_dx[NUM_ACTORS];	// horizontal velocity
sbyte actor_dy[NUM_ACTORS];	// vertical velocity

byte actor_attr[NUM_ACTORS]; // actor attributes

// main program
void main() {
  char i;	// actor index
  char oam_id;	// sprite ID
  const char start_x = (240/2) - ((NUM_ACTORS/4) * 16) + 8;
  char start_y = 256/2-20;
  
  char x = 0;
  char y = 0;
  
  for (y=0; y < 2; y++)
  {
    for (x=0; x < NUM_ACTORS/2; x++)
    {
      actor_x[i] = start_x + (x * 16) + (y * 8);
      actor_y[i] = start_y + (y * 16);
      actor_dx[i] = 0;//(rand() & 7) - 3;
      actor_dy[i] = 0;//(rand() & 7) - 3;
      actor_attr[i] = x | ((1-y) << 5);
      i++;
    }
  }
  
  for (y=0; y < 2; y++)
  {
    for (x=0; x < 19; x++)
    {
      vram_adr(NTADR_A(6+x,14+y));
      vram_put(0x01);
    }
  }
  
  // initialize PPU
  setup_graphics();
  // loop forever
  while (1) {
    // start with OAMid/sprite 0
    oam_id = 0;
    // draw and move all actors
    for (i=0; i<NUM_ACTORS; i++) {
      oam_id = oam_spr(actor_x[i], actor_y[i], 0x18, actor_attr[i], oam_id);
      actor_x[i] += actor_dx[i];
      actor_y[i] += actor_dy[i];
    }
    // hide rest of sprites
    // if we haven't wrapped oam_id around to 0
    if (oam_id!=0) oam_hide_rest(oam_id);
    // wait for next frame
    ppu_wait_frame();
  }
}
