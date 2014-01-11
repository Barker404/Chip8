#include <stdio.h>
#include <stdbool.h>
#include "chip8.h"

unsigned short opcode;

unsigned char memory[4096];	//4K memory

unsigned char V[16];		//general purpose registers (V[0xf] is 'carry flag')

unsigned short I; 			//index register
unsigned short pc;			//program counter, 0x000 to 0xFFF

/*
Memory Map
0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
0x050-0x0A0 - built in 4x5 pixel font set (0-F)
0x200-0xFFF - Program ROM and work RAM
*/

unsigned char key[16];		//HEX keypad (0x0-0xF) for input

unsigned short stack[16];
unsigned short sp;

void initialize();
{
	
}

void loadgame(char[]);
{
	
}

void emulateCycle();
{
	
}

void setKeys();
{
	
}