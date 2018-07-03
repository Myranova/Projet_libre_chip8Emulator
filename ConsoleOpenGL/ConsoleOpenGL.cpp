// ConsoleOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <bitset>
#include "CPU.h"
#include "Pixel.h"

void setBinary(std::bitset<16> & binary, const int binary_int[16])
{
	for (int i = 15; i >= 0; i--)
	{
		binary[i] = binary_int[i];
	}
}

int main()
{
	initScreen();
	//initPixel();
	//drawPixels();

	int cont = 1;
	int start = 0;
	int compter = 0;

	start = loadGame("./GAMES/BLITZ.ch8");
	std::cout << "value of start" << start << std::endl;
	if (start)
	{
		do {
			//cont = listen();

			drawPixels();
		} while (cont);
	}
	system("pause");
	return(1);
}

    

