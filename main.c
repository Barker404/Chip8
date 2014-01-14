#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "chip8.h"

#define SCALE 4;

SDL_Surface* screen;
SDL_Event event;
SDL_Rect *pixels[64][32];

SDL_PixelFormat *format;

unsigned int white;
unsigned int black; 

int main(int argc, char **argv)
{
	//Setup rendering and input
	setupGraphics();
	setupInput();

	//Initialize Chip8 system and load game into memory
	//initialize();
	//loadGame("pong");

	//Emulation loop
	while(gameRunning)
	{
		//Emulate one cycle
		//emulateCycle();

		//If draw flag set, update screen
		//if(drawFlag)
			drawGraphics();

		//Store key press state
		//setKeys();
	}

	SDL_FreeSurface(screen);
	SDL_Quit();

	return 0;
}

void setupGraphics()
{
	//Initialize SDL video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error: SDL failed to initialize");
		exit(1);
	}
	printf("Step 1 Success");

	//Set screen caption
	SDL_WM_SetCaption("Chip8 Emulator", "Chip8 Emulator");
	printf("Step 2 Success");
		
	//Set video mode to 64 x 32
	int h;
	h = 32*SCALE;
	int w;
	w = 64*SCALE;
	screen = SDL_SetVideoMode(w, h, 0, 0);
	if (screen == NULL)
	{
		printf("Error:Couldn't set screen to %d x %d", w, h);
		exit(1);
	}
	printf("Step 3 Success");

	for (int i = 0; i < 32; i++)		//looping across y
	{
		for (int j = 0; j < 64; j++)	//looping across x
		{
			pixels[j][i]->x = j*SCALE;
			pixels[j][i]->y = i*SCALE;
			pixels[j][i]->w = SCALE;
			pixels[j][i]->h = SCALE;	
		} 
	}

	format = screen->format;
	white = SDL_MapRGB(format, 255, 255, 255);
	black = SDL_MapRGB(format, 0, 0, 0);

	printf("Step 4 Success");	

	for (int i = 0; i < 32; i++)		//looping across y
	{
		for (int j = 0; j < 64; j++)	//looping across x
		{
			gfx[j][i]= j%2;	
		} 
	}

	SDL_Delay(1000);
}

void setupInput()
{
	
}

void drawGraphics()
{
	for (int i = 0; i < 32; i++)		//looping across y
	{
		for (int j = 0; j < 64; j++)	//looping across x
		{
			if (gfx[j][i] == 1)
				SDL_FillRect(screen, pixels[j][i], white);
			else
				SDL_FillRect(screen, pixels[j][i], black);	
		} 
	}
}