#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "chip8.h"

SDL_Surface* screen;
SDL_Event event;


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

	SDL_FreeSurface(screen);
	SDL_Quit();

	return 0;
}
void setupGraphics()
{
	//Initialize SDL video
	if (SDL_Init(SDL_INIT_VIDEO) <0)
	{
		printf("Error: SDL failed to initialize");
		exit(1);
	}
	printf("Step 1 Success");

	//Set screen caption
	SDL_WM_SetCaption("Chip8 Emulator", "Chip8 Emulator");
	printf("Step 2 Success");
		
	//Set video mode to 64 x 32
	screen = SDL_SetVideoMode(64,32,0,0);
	if (screen == NULL)
	{
		printf("Error:Couldn't set screen to 64 x 32");
		exit(1);
	}
	printf("Step 3 Success");
	SDL_Delay(1000);
}

void setupInput()
{
	
}

void drawGraphics()
{
	
}