//*****ToDo: loading game, I/O*****

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

unsigned short opcode;

unsigned char memory[4096];	//4K memory

unsigned char V[16];		//general purpose registers (V[0xf] is 'carry flag')

unsigned short I; 			//index register
unsigned short pc;			//program counter, 0x000 to 0xFFF

/*
Memory Map
0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
0x000-0x050 - built in 4x5 pixel font set (0-F)
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

#define OP1 ((opcode & 0xF000) >> 12) //Macros for getting bits of the opcode
#define OP2 ((opcode & 0x0F00) >> 8)
#define OP3 ((opcode & 0x00F0) >> 4)
#define OP4 ((opcode & 0x000F))

void initialize()
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



void loadgame(char name[])
{
	
}



void emulateCycle()
{
	//Fetch Opcode
	opcode = memory[pc] << 8 | memory[pc + 1];
	//Decode and Execute Opcode
	switch(OP1)
	{
		case 0x0:
			if (opcode == 0x00E0)
			{
				//Clears the screen
				for (int i = 0; i < 64; i++)
				{
					for (int j = 0; j < 32; j++)
					{
						gfx[i][j] = 0;
					}
				}
				drawFlag = true;
				pc += 2;
			}
			else if (opcode == 0x00EE)
			{
				//Returns from a subroutine
				sp--;
				pc = stack[sp];
			}
			else
			{
				//Calls RCA 1802 program at adress NNN
				//only used on old computers, ignored by modern interpreters
				printf("Error: opcode 0x0NNN occured unexpectedly");
			}
			break;

		case 0x1:
			//Jumps to address at NNN
			pc = (opcode & 0xFFF);
			break;

		case 0x2:
			//Calls subroutine at NNN
			stack[sp] = pc;
			sp++;
			pc = (opcode & 0xFFF);
			break;

		case 0x3:
			//Skips the next instruction if VX equals NN
			if (V[OP2] == (opcode & 0xFF))
				pc += 2;
			pc += 2;
			break;

		case 0x4:
			//Skips the next instruction if VX doesn't equal NN
			if (!V[OP3] == (opcode & 0xFF))
				pc += 2;
			pc += 2;
			break;

		case 0x5:
			if (OP4 == 0x0)
			{
				//Skips the next instruction if VX equals VY
				if (V[OP2] == V[OP3])
					pc += 2;
				pc += 2;
			}
			else
			{
				printf("Error: opcode 0x%X not specified\n", opcode);
			}
			break;

		case 0x6:
			//Sets VX to NN
			V[OP2] = (opcode & 0xFF);
			pc += 2;
			break;

		case 0x7:
			//Adds NN to VX
			V[OP2] += (opcode & 0xFF);
			pc += 2;
			break;

		case 0x8:
			switch (opcode & 0xF)
			{
				case 0x0:
					//Sets VX to the value of VY
					V[OP2] = V[OP3];
					pc += 2;
					break;
				case 0x1:
					//Sets VX to VX or VY
					V[OP2] |= V[OP3];
					pc += 2;
					break;
				case 0x2:
					//Sets VX to VX and VY
					V[OP2] &= V[OP3];
					pc += 2;
					break;
				case 0x3:
					//Sets VX to VX xor VY
					V[OP2] ^= V[OP3];
					break;
				case 0x4:
					//Adds VY to VX
					//VF is set to 1 when there's a carry, and to 0 when there isn't
					if (OP2 + OP3 > 0xFF)
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[OP2] += V[OP3];
					pc += 2;
					break;
				case 0x5:
					//VY is subtracted from VX
					//VF is set to 0 when there's a borrow, and 1 when there isn't
					if (OP2 < OP3)
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[OP2] -= V[OP3];
					pc += 2;
					break;
				case 0x6:
					//Shifts VX right by one
					//VF is set to the value of the least significant bit of VX before the shift
					V[0xF] = (OP2 & 0x1);
					V[OP2] >>= 1;
					pc += 2;
					break;
				case 0x7:
					//Sets VX to VY minus VX
					//VF is set to 0 when there's a borrow, and 1 when there isn't
					if (OP2 > OP3)
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[OP2] = V[OP3] - V[OP2];
					pc += 2;
					break;
				case 0xE:
					//Shifts VX left by one
					//VF is set to the value of the most significant bit of VX before the shift
					V[0xF] = (OP2 >> 3);
					V[OP2] <<= 1;
					pc += 2;
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
				if (V[OP2] != V[OP3])
					pc += 2;
				pc += 2;
			}
			else
			{
				printf("Error: opcode 0x%X not specified\n", opcode);
			}
			break;

		case 0xA:
			//Sets I to the address NNN
			I = (opcode & 0xFFF);
			pc += 2;
			break;

		case 0xB:
			//Jumps to the address NNN plus V0
			pc = ((opcode & 0xFFF) + V[0]);
			break;

		case 0xC:
			//Sets VX to a random number and NN
			;
			int divisor = RAND_MAX / 256;
			int num;

			do
				num = rand() / divisor;
			while (num > 255);

			V[OP2] = num & (opcode & 0xFF);
			break;

		case 0xD:
			//Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels
			//Each row of 8 pixels is read as bit-coded (with the most significant bit of each byte displayed on the left) starting from memory location I; I value doesn't change after the execution of this instruction
			//VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn't happen
			;
			unsigned short xpos = V[OP2];
			unsigned short ypos = V[OP3];
			unsigned short row;

			for (int i = 0; i < OP4; i++)		//looping across y
			{
				row = memory[I + i];
				for (int j = 0; j < 8; j++)		//looping across x
				{
					if (row & (0x80 >> j))
					{
						if (gfx[xpos + j][ypos + i])
						{
							V[0xF] = 1;
							gfx[xpos + j][ypos + i] = 0;
						}
						else
							gfx[xpos + j][ypos + i] = 1;
					}
				}
			}

			drawFlag = true;
			pc += 2;	
			break;

		case 0xE:
			switch(opcode & 0xFF)
			{
				case 0x9E:
					//Skips the next instruction if the key stored in VX is pressed
					if (key[V[OP2]])
						pc += 2;
					pc += 2;
					break;
				case 0xA1:
					//Skips the next instruction if the key stored in VX isn't pressed
					if (!key[V[OP2]])
						pc += 2;
					pc += 2;
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
					V[OP2] = delay_timer;
					pc += 2;
					break;
				case 0x0A:
					//A key press is awaited, and then stored in VX
					;
					int pressed;
					int i = 0;
					while (!pressed & i++ < 16)
					{
						if(key[i])
							pressed = 1;
							V[OP2] = i;
							pc += 2;
					}
					break;
				case 0x15:
					//Sets the delay timer to VX
					delay_timer = V[OP2];
					pc += 2;
					break;
				case 0x18:
					//Sets the sound timer to VX
					sound_timer = V[OP2];
					pc += 2;
					break;
				case 0x1E:
					//Adds VX to I
					I += V[OP2];
					pc += 2;
					break;
				case 0x29:
					//Sets I to the location of the sprite for the character in VX
					//Characters 0-F (in hexadecimal) are represented by a 4x5 font
					if (V[OP2] <= 0xF)
					{
						I = V[OP2] * 5;
						pc += 2;
					}
					else
						printf("Error: fontset character out of range: %x",V[OP2]);
					break;
				case 0x33:
					//Stores the Binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2
					//In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2
					memory[I] = V[OP2] / 100;
					memory[I+1] = (V[OP2] / 10) % 10;
					memory[I+2] = (V[OP2] % 100) %10;
					pc += 2;
					break;
				case 0x55:
					//Stores V0 to VX in memory starting at address I
					for (int i = 0; i <= OP2; i++)
						memory[I + i] = V[i];
					pc += 2;
					break;
				case 0x65:
					//Fills V0 to VX with values from memory starting at address I
					for (int i = 0; i <= OP2; i++)
						V[i] = memory[I + i];
					pc += 2;
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



void setKeys()
{
	
}
