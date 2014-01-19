#include <stdbool.h>

unsigned char gfx[64][32];	//2048 pixels on screen

unsigned char delay_timer;	//60Hz
unsigned char sound_timer;	//60Hz

unsigned char key[16];		//HEX keypad (0x0-0xF) for input

bool drawFlag;				//Set by 0x00E0 (clear screen) and 0xDXYN (draw sprite)
bool gameRunning;
//main.c functions
void setupGraphics();
void setupInput();
void drawGraphics();

//chip8.c functions
void initialize();
void loadGame(char name[]); //loads the game "name"
void emulateCycle();		//emulates a single game cycle
void setKeys();				//sets input keys