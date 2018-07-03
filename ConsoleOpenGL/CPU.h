#pragma once

#ifndef CPU_H_
# define CPU_H

# define MEMORY_SIZE 4096
# define START_ADRESS 512
# define NB_OPCODE 35

#include <iostream>
#include <stdlib.h>

typedef struct
{
	uint8_t memory[MEMORY_SIZE];
	uint8_t register_chip[16];
	uint16_t I;
	uint16_t jump[16];
	uint8_t nb_jump;
	uint8_t game_compter;
	uint8_t sound_compter;
	uint16_t pc;
} CPU;

extern CPU cpu; //declare the CPU
extern uint8_t memory[4096];

void initCPU();
void decompter();
uint16_t getOpcode();
uint8_t getAction(uint16_t opcode);

typedef struct
{
	uint16_t mask[NB_OPCODE];
	uint16_t id[NB_OPCODE];

} JUMP;

extern JUMP jp;

void initJump();
void interpretOpcode(uint16_t);
int	loadGame(char *game_name);

#endif /* CPU_H*/