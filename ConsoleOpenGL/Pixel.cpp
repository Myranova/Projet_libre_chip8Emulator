#include "stdafx.h"
#include "Pixel.h"
#include "CPU.h"

sf::RenderWindow* screen;
sf::RectangleShape* square[2];

PIXEL pixel_tab[l][L];


void initScreen()
{
	screen = NULL;
	square[0] = NULL;
	square[1] = NULL;

	screen = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "CHIP");
	if (screen == NULL)
	{
		std::cerr << "error loading video mode" << std::endl;
		exit(EXIT_FAILURE);
	}

	sf::RectangleShape *pixel_black = new sf::RectangleShape(); //black pixel
	pixel_black->setFillColor(sf::Color::Black);
	pixel_black->setSize(sf::Vector2f(DIMENSION_PIXEL, DIMENSION_PIXEL));
	if (pixel_black == NULL)
	{
		std::cerr << "error loading surface" << std::endl;
		exit(EXIT_FAILURE);
	}
	square[0] = pixel_black;

	sf::RectangleShape *pixel_white = new sf::RectangleShape();
	pixel_white->setFillColor(sf::Color::White);
	pixel_white->setSize(sf::Vector2f(DIMENSION_PIXEL, DIMENSION_PIXEL));
	if (pixel_white == NULL)
	{
		std::cerr << "error loading surface" << std::endl;
		exit(EXIT_FAILURE);
	}
	square[1] = pixel_white;
}

void initPixel()
{
	int x = 0;
	int y = 0;

	for (x = 0; x < l; x++)
	{
		for (y = 0; y < L; y++)
		{
			if (x % (y + 1) == 0)
			{
				pixel_tab[x][y].position = *square[0];
				pixel_tab[x][y].position.setPosition(x * DIMENSION_PIXEL, y * DIMENSION_PIXEL);
			}
			else
			{
				pixel_tab[x][y].position = *square[1];
				pixel_tab[x][y].position.setPosition(x * DIMENSION_PIXEL, y * DIMENSION_PIXEL);
			}
		}
	}
}

void clearScreen()
{
	int x = 0;
	int y = 0;

	for (x = 0; x < l; x++)
	{
		for (y = 0; y < L; y++)
		{
			pixel_tab[x][y].color = BLACK;
		}
	}
}
void drawPixel(PIXEL pixel)
{
	screen->draw(pixel.position);
}


void updateScreen()
{
	int x = 0;
	int y = 0;

	for (x = 0; x < l; x++)
	{
		for (y = 0; y < L; y++)
		{
			drawPixel(pixel_tab[x][y]);
		}
	}
}

void drawPixels()
{
	int x = 0;

	int y = 0;
	while (screen->isOpen())
	{
		sf::Event event;

		while (screen->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				screen->close();
		}
		screen->clear(sf::Color::Black);
		for (int i = 0; i < SPEED_CPU; i++)
		{
			interpretOpcode(getOpcode());
		}
		//drawPixels();
		//initPixel();
		updateScreen();
		decompter();
		Sleep(16);
		screen->display();
	}
}

void drawScreen(uint8_t b1, uint8_t b2, uint8_t b3)
{
	/*b1 = bit de poids faible (venant de l'opcode)
	/qui réprésente un numéro ou index de registre*/

	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t k = 0;
	uint8_t codage = 0;
	uint8_t j = 0;
	uint8_t decalage = 0;

	std::cout << "call to draw screen" << std::endl;
	for (k = 0; k < b1; k++)
	{
		codage = cpu.memory[cpu.I + k]; // On récupère le codage de la ligne à dessiner

		y = (cpu.register_chip[b2] + k) % L; //On calcule l'ordonnée de la ligne à dessiner
											 //On ne doit pas dépasser L;
		for (j = 0, decalage = 7; j < 8; j++, decalage--)
		{
			x = (cpu.register_chip[b3] + j) % l; // On récupère le X

			if (((codage) & (0x1 << decalage)) != 0)
			{
				if (pixel_tab[x][y].color == WHITE)
				{
					if (pixel_tab[x][y].color == WHITE)
					{
						pixel_tab[x][y].color == BLACK;
						cpu.register_chip[0xF] = 1; //colision voir dans le manuel chip8 registres
					}
					else
					{
						pixel_tab[x][y].color = WHITE;
					}
				}
			}
		}
	}
}