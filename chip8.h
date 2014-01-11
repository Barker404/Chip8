#include <stdbool.h>

//==========to move?==========

unsigned char gfx[64][32];	//2048 pixels on screen

unsigned char delay_timer;	//60Hz
unsigned char sound_timer;	//60Hz

//==========end block==========

bool drawFlag;				//Set by 0x00E0 (clear screen) and 0xDXYN (draw sprite)
bool gameRunning;

void initialize();
void loadgame(char[] name); //loads the game "name" (from local directory?)
void emulateCycle();		//emulates a single game cycle
void setKeys();				//sets input keys