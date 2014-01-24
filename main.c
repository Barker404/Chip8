#include <stdio.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include "chip8.h"

#define SCALE 4				//Scale to increase screen size by
#define TARGET 120			//Target cycles per second

SDL_Surface* screen; 		//Surface to draw on
SDL_Event event;
SDL_Rect pixels[64][32];	//Actual rectangles to be drawn

SDL_PixelFormat *format;	//Format of screen

unsigned int white;
unsigned int black; 

unsigned int cycles; 		//Cycles passed
unsigned long startTime;	//Time in ms since SDL initialized (taken at start of loop)
unsigned int cycleTime;		//Time taken for one cycle

unsigned long fpsStartTime;	//Time 60 frames ago
unsigned int fps;			//Frames in last second


int main(int argc, char **argv)
{
	//Setup rendering and input
	cycles = 0;
	setupGraphics();
	setupInput();

	//Initialize Chip8 system and load game into memory
	initialize();
	loadGame("PONG");

	//Emulation loop
	gameRunning = 1;
	while(gameRunning)
	{
		//Get current time
		startTime = SDL_GetTicks();

		if (cycles % TARGET == 0 && cycles != 0)
		{
			//Get fps over last 60 frames
			fps = ((TARGET * 1000) / (SDL_GetTicks() - fpsStartTime));
			printf("%i\n", fps);
			fpsStartTime = SDL_GetTicks();
		}

		//Emulate one cycle
		emulateCycle();

		//If draw flag set, update screen
		if(drawFlag)
			drawGraphics();

		//Store key press state
		setKeys();

		cycles++;
		//Wait until 16ms passed total (~60Hz)	
		cycleTime = SDL_GetTicks() - startTime;
		SDL_Delay((1000/TARGET)-cycleTime);
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

	//Set screen caption
	SDL_WM_SetCaption("Chip8 Emulator", "Chip8 Emulator");
		
	//Set video mode to 64 x 32 times the scale
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

	//Setup rectangles
	for (int i = 0; i < 32; i++)		//looping across y
	{
		for (int j = 0; j < 64; j++)	//looping across x
		{
			pixels[j][i].x = j*SCALE;
			pixels[j][i].y = i*SCALE;
			pixels[j][i].w = SCALE;
			pixels[j][i].h = SCALE;	
		} 
	}

	//Setup colours
	format = screen->format;
	white = SDL_MapRGB(format, 255, 255, 255);
	black = SDL_MapRGB(format, 0, 0, 0);

	//Set inital (fake) fps
	fps = 60;
}

void setupInput()
{
	
}

void setKeys()
{
	while( SDL_PollEvent(&event) )
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				//Key press detected
				switch(event.key.keysym.sym)
				{
					case SDLK_1:
						key[0x1] = 1;
						break;
					case SDLK_2:
						key[0x2] = 1;
						break;
					case SDLK_3:
						key[0x3] = 1;
						break;
					case SDLK_4:
						key[0xc] = 1;
						break;
					case SDLK_q:
						key[0x4] = 1;
						break;
					case SDLK_w:
						key[0x5] = 1;
						break;
					case SDLK_e:
						key[0x6] = 1;
						break;
					case SDLK_r:
						key[0xd] = 1;
						break;
					case SDLK_a:
						key[0x7] = 1;
						break;
					case SDLK_s:
						key[0x8] = 1;
						break;
					case SDLK_d:
						key[0x9] = 1;
						break;
					case SDLK_f:
						key[0xe] = 1;
						break;
					case SDLK_z:
						key[0xa] = 1;
						break;
					case SDLK_x:
						key[0x0] = 1;
						break;
					case SDLK_c:
						key[0xb] = 1;
						break;
					case SDLK_v:
						key[0xf] = 1;
						break;
					default:
						break;
				}
				break;

			case SDL_KEYUP:
				//Key release detected
				switch(event.key.keysym.sym)
				{
					case SDLK_1:
						key[0x1] = 0;
						break;
					case SDLK_2:
						key[0x2] = 0;
						break;
					case SDLK_3:
						key[0x3] = 0;
						break;
					case SDLK_4:
						key[0xc] = 0;
						break;
					case SDLK_q:
						key[0x4] = 0;
						break;
					case SDLK_w:
						key[0x5] = 0;
						break;
					case SDLK_e:
						key[0x6] = 0;
						break;
					case SDLK_r:
						key[0xd] = 0;
						break;
					case SDLK_a:
						key[0x7] = 0;
						break;
					case SDLK_s:
						key[0x8] = 0;
						break;
					case SDLK_d:
						key[0x9] = 0;
						break;
					case SDLK_f:
						key[0xe] = 0;
						break;
					case SDLK_z:
						key[0xa] = 0;
						break;
					case SDLK_x:
						key[0x0] = 0;
						break;
					case SDLK_c:
						key[0xb] = 0;
						break;
					case SDLK_v:
						key[0xf] = 0;
						break;
					default:
						break;
				}
				break;

			case SDL_QUIT:
				gameRunning = 0;
				break;
			default:
				break;
		}
	}
}

void drawGraphics()
{
	for (int i = 0; i < 32; i++)		//looping across y
	{
		for (int j = 0; j < 64; j++)	//looping across x
		{
			if (gfx[j][i] == 1)
				SDL_FillRect(screen, &pixels[j][i], white);
			else
				SDL_FillRect(screen, &pixels[j][i], black);
		} 
	}
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}