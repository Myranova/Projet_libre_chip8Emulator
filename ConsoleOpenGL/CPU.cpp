#include "stdafx.h"
#include "CPU.h"
#include "Pixel.h"

CPU cpu;
JUMP jp;

void initCPU()
{
	uint16_t i = 0;

	for (i = 0; i<MEMORY_SIZE; i++) 
	{
		cpu.memory[i] = 0;
	}

	for (i = 0; i<16; i++)
	{
		cpu.register_chip[i] = 0;
		cpu.jump[i] = 0;
	}

	cpu.pc = START_ADRESS;
	cpu.nb_jump = 0;
	cpu.game_compter = 0;
	cpu.sound_compter = 0;
	cpu.I = 0;

	initJump();
}

void decompter()
{
	if (cpu.game_compter > 0)
		cpu.game_compter--;

	if (cpu.sound_compter > 0)
		cpu.sound_compter--;
}

uint16_t getOpcode()
{
	std::cout << "get opcode : " << unsigned(cpu.memory[cpu.pc] << 8) + cpu.memory[cpu.pc + 1] << std::endl;
	return (cpu.memory[cpu.pc] << 8) + cpu.memory[cpu.pc + 1];
	
}

void initJump()
{
	jp.mask[0] = 0x0000; jp.id[0] = 0x0FFF;          /* 0NNN */
	jp.mask[1] = 0xFFFF; jp.id[1] = 0x00E0;          /* 00E0 */
	jp.mask[2] = 0xFFFF; jp.id[2] = 0x00EE;          /* 00EE */
	jp.mask[3] = 0xF000; jp.id[3] = 0x1000;          /* 1NNN */
	jp.mask[4] = 0xF000; jp.id[4] = 0x2000;          /* 2NNN */
	jp.mask[5] = 0xF000; jp.id[5] = 0x3000;          /* 3XNN */
	jp.mask[6] = 0xF000; jp.id[6] = 0x4000;          /* 4XNN */
	jp.mask[7] = 0xF00F; jp.id[7] = 0x5000;          /* 5XY0 */
	jp.mask[8] = 0xF000; jp.id[8] = 0x6000;          /* 6XNN */
	jp.mask[9] = 0xF000; jp.id[9] = 0x7000;          /* 7XNN */
	jp.mask[10] = 0xF00F; jp.id[10] = 0x8000;          /* 8XY0 */
	jp.mask[11] = 0xF00F; jp.id[11] = 0x8001;          /* 8XY1 */
	jp.mask[12] = 0xF00F; jp.id[12] = 0x8002;          /* 8XY2 */
	jp.mask[13] = 0xF00F; jp.id[13] = 0x8003;          /* BXY3 */
	jp.mask[14] = 0xF00F; jp.id[14] = 0x8004;          /* 8XY4 */
	jp.mask[15] = 0xF00F; jp.id[15] = 0x8005;          /* 8XY5 */
	jp.mask[16] = 0xF00F; jp.id[16] = 0x8006;          /* 8XY6 */
	jp.mask[17] = 0xF00F; jp.id[17] = 0x8007;          /* 8XY7 */
	jp.mask[18] = 0xF00F; jp.id[18] = 0x800E;          /* 8XYE */
	jp.mask[19] = 0xF00F; jp.id[19] = 0x9000;          /* 9XY0 */
	jp.mask[20] = 0xF000; jp.id[20] = 0xA000;          /* ANNN */
	jp.mask[21] = 0xF000; jp.id[21] = 0xB000;          /* BNNN */
	jp.mask[22] = 0xF000; jp.id[22] = 0xC000;          /* CXNN */
	jp.mask[23] = 0xF000; jp.id[23] = 0xD000;          /* DXYN */
	jp.mask[24] = 0xF0FF; jp.id[24] = 0xE09E;          /* EX9E */
	jp.mask[25] = 0xF0FF; jp.id[25] = 0xE0A1;          /* EXA1 */
	jp.mask[26] = 0xF0FF; jp.id[26] = 0xF007;          /* FX07 */
	jp.mask[27] = 0xF0FF; jp.id[27] = 0xF00A;          /* FX0A */
	jp.mask[28] = 0xF0FF; jp.id[28] = 0xF015;          /* FX15 */
	jp.mask[29] = 0xF0FF; jp.id[29] = 0xF018;          /* FX18 */
	jp.mask[30] = 0xF0FF; jp.id[30] = 0xF01E;          /* FX1E */
	jp.mask[31] = 0xF0FF; jp.id[31] = 0xF029;          /* FX29 */
	jp.mask[32] = 0xF0FF; jp.id[32] = 0xF033;          /* FX33 */
	jp.mask[33] = 0xF0FF; jp.id[33] = 0xF055;          /* FX55 */
	jp.mask[34] = 0xF0FF; jp.id[34] = 0xF065;
}

uint8_t getAction(uint16_t opcode)
{
	uint8_t action;
	uint16_t result;

	for (action = 0; action < NB_OPCODE; action++)
	{
		result = (jp.mask[action] & opcode);

		if (result == jp.id[action])
			break;
	} 
	return (action); //renvoie l'indice de l'action à effectuer
}

int	loadGame(char *game_name)
{
	FILE * game = NULL;
	size_t size_game;
	game = fopen(game_name, "rb");
	if (game != NULL)
	{
		fseek(game, 0L, SEEK_END);
		size_game = ftell(game);
		fseek(game, 0, SEEK_SET);
		std::cout << "size game" << size_game << std::endl;
		size_t sizefread = fread(&cpu.memory[START_ADRESS], sizeof(uint8_t), sizeof(uint8_t) * (MEMORY_SIZE - START_ADRESS), game);

		std::cout << "sizefread : " << sizefread << std::endl;
		fclose(game);
		return (1);
	}
	else {
		std::cerr << "Problème d'ouverture de fichier" << std::endl;
		return (0);
	}
}

void interpretOpcode(uint16_t opcode)
{
	uint8_t b4, b3, b2, b1;

	//Comprendre cette partie importante
	b3 = (opcode&(0x0F00)) >> 8;
	b2 = (opcode&(0x00F0)) >> 4;
	b1 = (opcode&(0x000F));
	b4 = getAction(opcode);

	std::cout << "B1 : " << unsigned(b1) << std::endl;
	std::cout << "B2 : " << unsigned(b2) << std::endl;
	std::cout << "B3 : " << unsigned(b3) << std::endl;
	std::cout << "B4 : " << unsigned(b4) << std::endl << std::endl;;
	switch (b4)
	{
				case 0: {
				// not implemented yet
				break;
		}
		case 1: {
			clearScreen();
			break;
		}
		case 2: {
			if (cpu.nb_jump > 0)
				cpu.nb_jump--;
			cpu.pc = cpu.jump[cpu.nb_jump]; //understand it
			break;
		}
		case 3: { //1NNN: effectue un saut à l'adresse 1NNN
			cpu.pc = (b3 << 8) + (b2 << 4) + b1; //on prend le nombre NNN (pour le saut)
			cpu.pc -= 2; //on verra pourquoi à la fin

			break;
		}
		case 4: { //2NNN: appelle le sous-programme en NNN, mais on revient ensuite
			cpu.jump[cpu.nb_jump] = cpu.pc; //on reste là où on était
			if (cpu.nb_jump<15)
			{
				cpu.nb_jump++;
			}
			cpu.pc = (b3 << 8) + (b2 << 4) + b1; //on prend le nombre NNN (pour le saut)
			cpu.pc -= 2; //on verra pourquoi à la fin
			break;
		}
		case 5: {//3XNN saute l'instruction suivante si VX est égal à NN.
			if (cpu.register_chip[b3] == ((b2 << 4) + b1))
			{
				cpu.pc += 2;
			}
			break;
		}
		case 6: {//4XNN saute l'instruction suivante si VX et NN ne sont pas égaux.
			if (cpu.register_chip[b3] != ((b2 << 4) + b1))
			{
				cpu.pc += 2;
			}
			break;
		}
		case 7: {
			//5XY0 saute l'instruction suivante si VX et VY sont égaux.
			if (cpu.register_chip[b3] == cpu.register_chip[b2])
			{
				cpu.pc += 2;
			}
			break;
		}
		case 8: {
			//6XNN définit VX à NN.
			cpu.register_chip[b3] = (b2 << 4) + b1;
			break;
		}
		case 9: {
			//7XNN ajoute NN à VX.
			cpu.register_chip[b3] += (b2 << 4) + b1;
			break;
		}
		case 10: {
			//8XY0 définit VX à la valeur de VY.
			cpu.register_chip[b3] = cpu.register_chip[b2];
			break;
		}
		case 11: {
			//8XY1 définit VX à VX OR VY.
			cpu.register_chip[b3] = cpu.register_chip[b3] | cpu.register_chip[b2];
			break;
		}
		case 12: {
			//8XY2 définit VX à VX AND VY.
			cpu.register_chip[b3] = cpu.register_chip[b3] & cpu.register_chip[b2];
			break;
		}
		case 13: {
			//8XY3 définit VX à VX XOR VY.
			cpu.register_chip[b3] = cpu.register_chip[b3] ^ cpu.register_chip[b2];
			break;
		}
		case 14: {

			//8XY4 ajoute VY à VX. VF est mis à 1 quand il y a un dépassement de mémoire (carry), et à 0 quand il n'y en pas.
			if ((cpu.register_chip[b3] + cpu.register_chip[b2])>255)
			{
				cpu.register_chip[0xF] = 1; //cpu.register_chip[15]
			}
			else
			{
				cpu.register_chip[0xF] = 0; //cpu.register_chip[15]
			}
			cpu.register_chip[b3] += cpu.register_chip[b2];
			break;
		}
		case 15: {
			//8XY5 VY est soustraite de VX. VF est mis à 0 quand il y a un emprunt, et à 1 quand il n'y a en pas.
			if ((cpu.register_chip[b3]<cpu.register_chip[b2]))
			{
				cpu.register_chip[0xF] = 0; //cpu.register_chip[15]
			}

			else
			{
				cpu.register_chip[0xF] = 1; //cpu.register_chip[15]
			}
			cpu.register_chip[b3] -= cpu.register_chip[b2];
			break;
		}
		case 16: {
			//8XY6 décale (shift) VX à droite de 1 bit. VF est fixé à la valeur du bit de poids faible de VX avant le décalage.
			cpu.register_chip[0xF] = (cpu.register_chip[b3] & (0x01));
			cpu.register_chip[b3] = (cpu.register_chip[b3] >> 1);
			break;
		}
		case 17: {
			//8XY7 VX = VY - VX. VF est mis à 0 quand il y a un emprunt et à 1 quand il n'y en a pas
			if ((cpu.register_chip[b2]<cpu.register_chip[b3]))
			{
				cpu.register_chip[0xF] = 0; //cpu.register_chip[15]
			}
			else
			{
				cpu.register_chip[0xF] = 1; //cpu.register_chip[15]
			}
			cpu.register_chip[b3] = cpu.register_chip[b2] - cpu.register_chip[b3];

			break;
		}
		case 18: {
			//8XYE décale (shift) VX à gauche de 1 bit. VF est fixé à la valeur du bit de poids fort de VX avant le décalage.
			cpu.register_chip[0xF] = (cpu.register_chip[b3] >> 7);

			cpu.register_chip[b3] = (cpu.register_chip[b3] << 1);
			break;
		}
		case 19: {
			//9XY0 saute l'instruction suivante si VX et VY ne sont pas égaux.
			if (cpu.register_chip[b3] != cpu.register_chip[b2])
			{
				cpu.pc += 2;
			}
			break;
		}
		case 20: {
			//ANNN affecte NNN à I.
			cpu.I = (b3 << 8) + (b2 << 4) + b1;
			break;
		}
		case 21: {
			//BNNN passe à l'adresse NNN + V0.
			cpu.pc = (b3 << 8) + (b2 << 4) + b1 + cpu.register_chip[0];

			cpu.pc -= 2;
			break;
		}
		case 22: {
			//CXNN définit VX à un nombre aléatoire inférieur à NN.
			cpu.I = (b3 << 8) + (b2 << 4) + b1;
			break;
		}
		case 23: {
			//DXYN dessine un sprite aux coordonnées (VX, VY).
			std::cout << "draw screen" << std::endl;
			drawScreen(b1, b2, b3);
			break;
		}
		case 24: {
			//EX9E saute l'instruction suivante si la clé stockée dans VX est pressée.
			break;
		}
		case 25: {
			//EXA1 saute l'instruction suivante si la clé stockée dans VX n'est pas pressée.
			break;
		}
		case 26: {
			//FX07 définit VX à la valeur de la temporisation.
			cpu.register_chip[b3] = cpu.game_compter;
			break;
		}
		case 27: {
			//FX0A attend l'appui sur une touche et la stocke ensuite dans VX.
			break;
		}
		case 28: {
			//FX15 définit la temporisation à VX.
			cpu.sound_compter = cpu.register_chip[b3];
			break;
		}
		case 29: {
			//FX18 définit la minuterie sonore à VX.
			cpu.sound_compter = cpu.register_chip[b3];
			break;
		}
		case 30: {
			//FX1E ajoute à VX I. VF est mis à 1 quand il y a overflow (I+VX>0xFFF), et à 0 si tel n'est pas le cas.
			if ((cpu.I + cpu.register_chip[b3])>0xFFF)
			{
				cpu.register_chip[0xF] = 1;
			}
			else
			{
				cpu.register_chip[0xF] = 0;
			}
			cpu.I += cpu.register_chip[b3];
			break;
		}
		case 31: {
			//FX29 définit I à l'emplacement du caractère stocké dans VX. Les caractères 0-F (en hexadécimal) sont représentés par une police 4x5.
			cpu.I = cpu.register_chip[b3] * 5;
			break;
		}
		case 32: {
			//FX33 stocke dans la mémoire le code décimal représentant VX (dans I, I+1, I+2).
			cpu.memory[cpu.I] = (cpu.register_chip[b3] - cpu.register_chip[b3] % 100) / 100;
			cpu.memory[cpu.I + 1] = (((cpu.register_chip[b3] - cpu.register_chip[b3] % 10) / 10) % 10);
			cpu.memory[cpu.I + 2] = cpu.register_chip[b3] - cpu.memory[cpu.I] * 100 - 10 * cpu.memory[cpu.I + 1];
			break;
		}
		case 33: {
			//FX55 stocke V0 à VX en mémoire à partir de l'adresse I.
			uint8_t i = 0;

			for (i = 0; i <= b3; i++)
			{
				cpu.memory[cpu.I + i] = cpu.register_chip[i];
			}
			break;
		}
		case 34: {
			//FX65 remplit V0 à VX avec les valeurs de la mémoire à partir de l'adresse I.
			uint8_t i = 0;

			for (i = 0; i <= b3; i++)
			{
				cpu.register_chip[i] = cpu.memory[cpu.I + i];
			}

			break;
		}
		default: {//si ça arrive, il y un truc qui cloche
			break;
		}
	}
	cpu.pc += 2; //on passe au prochain opcode , on fait + 2 car l'opcode est composé de 2 octets
}