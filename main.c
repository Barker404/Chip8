#include <stdio.h>
#include <stdbool.h>
#include "chip8.h"
//input and output here


int main(int argc, char **argv)
{
	//Setup rendering and input
	setupGraphics();
	setupInput();

	//Initialize Chip8 system and load game into memory
	initialize();
	loadGame("pong");

	//Emulation loop
	while(gameRunning)
	{
		//Emulate one cycle
		emulateCycle();

		//If draw flag set, update screen
		if(drawFlag)
			drawGraphics();

		//Store key press state
		setKeys();
	}

	return 0;
}

void setupGraphics()
{
	
}

void setupInput()
{
	
}

void drawGraphics()
{
	
}