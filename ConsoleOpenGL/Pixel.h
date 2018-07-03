#pragma once

#ifndef PIXEL_H_
# define PIXEL_H_

# include <SFML/Graphics.hpp>
# include <iostream>

# define BLACK 0
# define WHITE 1
# define l 64
# define L 32
# define DIMENSION_PIXEL 20
# define WIDTH l*DIMENSION_PIXEL //Dimension
# define HEIGHT L*DIMENSION_PIXEL //Dimension
# define FPS 16
# define SPEED_CPU 4

void initPixel();
void initScreen();
void drawPixels();
void drawScreen(uint8_t b1, uint8_t b2, uint8_t b3);
void clearScreen();

typedef struct
{
	sf::RectangleShape position; //pixel
	uint8_t color;
} PIXEL;

extern sf::RenderWindow* screen;
extern sf::RectangleShape* square[2];

extern PIXEL pixel_tab[l][L];

void updateScreen();

#endif /* PIXEL_H_ */