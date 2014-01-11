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

unsigned char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void initialize();
{
	pc		= 0x200;	//Program counter starts at 0x200
	opcode	= 0;		//Reset opcode
	I		= 0;		//Reset index pointer
	sp		= 0;		//Reset stack pointer

	//Clear display
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			gfx[i][j] = 0;
		}
	}
	//Clear stack
	for (int i = 0; i < 16; i++)
		stack[i] = 0;

	//Clear registers V0-VF
	for (int i = 0; i < 16; i++)
		V[i] = 0;

	//Clear memory
	for (int i = 0; i < 4096; i++)
		memory[i] = 0;

	//Load fontset					*****0 to 80 or 80 to 160?*****
	for (int i = 0; i < 80; i++)
		memory[i] = chip8_fontset[i];

	//Reset timers
	delay_timer = 0;
	sound_timer = 0;
}



void loadgame(char[]);
{
	
}



void emulateCycle();
{
	//Fetch Opcode
	opcode = memory[pc] << 8 | memory[pc + 1];
	//Decode and Execute Opcode
	switch(opcode >> 12)
	{
		case 0x0:
			if (opcode == 0x00E0)
			{
				//Clears the screen

			}
			else if (opcode == 0x00EE)
			{
				//Returns from a subroutine

			}
			else
			{
				//Calls RCA 1802 program at adress NNN

			}
			break;

		case 0x1:
			//Jumps to address at NNN

			break;

		case 0x2:
			//Calls subroutine at NNN

			break;

		case 0x3:
			//Skips the next instruction if VX equals NN

			break;

		case 0x4:
			//Skips the next instruction if VX doesn't equal NN

			break;

		case 0x5:
			if ((opcode & 0xF) == 0x0)
			{
				//Skips the next instruction if VX equals VY

			}
			else
			{
				printf("Error: opcode 0x%X not specified\n", opcode);
			}
			break;

		case 0x6:
			//Sets VX to NN

			break;

		case 0x7:
			//Adds NN to VX

			break;

		case 0x8:
			switch (opcode & 0xF)
			{
				case 0x0:
					//Sets VX to the value of VY

					break;
				case 0x1:
					//Sets VX to VX or VY

					break;
				case 0x2:
					//Sets VX to VX and VY

					break;
				case 0x3:
					//Sets VX to VX xor VY

					break;
				case 0x4:
					//Adds VY to VX
					//VF is set to 1 when there's a carry, and to 0 when there isn't

					break;
				case 0x5:
					//VY is subtracted from VX
					//VF is set to 0 when there's a borrow, and 1 when there isn't

					break;
				case 0x6:
					//Shifts VX right by one
					//VF is set to the value of the least significant bit of VX before the shift

					break;
				case 0x7:
					//Sets VX to VY minus VX
					//VF is set to 0 when there's a borrow, and 1 when there isn't

					break;
				case 0xE:
					//Shifts VX left by one
					//VF is set to the value of the most significant bit of VX before the shift

					break;
				default:
					printf("Error: opcode 0x%X not specified\n", opcode);
					break;
			}
			break;

		case 0x9:
			if ((opcode & 0xF) == 0)
			{
				//Skips the next instruction if VX doesn't equal VY

			}
			else
			{
				printf("Error: opcode 0x%X not specified\n", opcode);
			}
			break;

		case 0xA:
			//Sets I to the address NNN

			break;

		case 0xB:
			//Jumps to the address NNN plus V0

			break;

		case 0xC:
			//Sets VX to a random number and NN

			break;

		case 0xD:
			//Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels
			//Each row of 8 pixels is read as bit-coded (with the most significant bit of each byte displayed on the left) starting from memory location I; I value doesn't change after the execution of this instruction
			//VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn't happen
			
			break;

		case 0xE:
			switch(opcode & 0xFF)
			{
				case 0x9E:
					//Skips the next instruction if the key stored in VX is pressed

					break;
				case 0xA1:
					//Skips the next instruction if the key stored in VX isn't pressed
				
					break;
				default:
					printf("Error: opcode 0x%X not specified\n", opcode);
					break;
			}	
			break;

		case 0xF:
			switch(opcode & 0xFF)
			{
				case 0x07:
					//Sets VX to the value of the delay timer

					break;
				case 0x0A:
					//A key press is awaited, and then stored in VX

					break;
				case 0x15:
					//Sets the delay timer to VX

					break;
				case 0x18:
					//Sets the sound timer to VX

					break;
				case 0x1E:
					//Adds VX to I

					break;
				case 0x29:
					//Sets I to the location of the sprite for the character in VX
					//Characters 0-F (in hexadecimal) are represented by a 4x5 font

					break;
				case 0x33:
					//Stores the Binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2
					//In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2

					break;
				case 0x55:
					//Stores V0 to VX in memory starting at address I

					break;
				case 0x65:
					//Fills V0 to VX with values from memory starting at address I

					break;
				default:
					printf("Error: opcode 0x%X not specified\n", opcode);
					break;
			}

			break;

		default:
			printf("Error: opcode 0x%X not specified\n", opcode);
			break;
	}


	//Update timers
	if (delay_timer > 0)
		delay_timer--;

	if (sound_timer == 1)
		printf("BEEP!\a\n");
		sound_timer--;
}



void setKeys();
{
	
}