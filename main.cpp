//Names: Andrew Trageser, Johnny Hein, Daniel , Gregory Bellinger
//Assignment: PA9
//Title: Seed Drop
//Description: Endless Verticle Scroller 

#include <iostream>
#include <SDL.h>
#include <time.h>
#include <string.h>


using std::cout;
using std::endl;

#include "Game.h"

Game *game = nullptr;

int main(int argc, char *argv[])
{
	srand(time(NULL));

	const int FPS = 60; // frame rates
	const int frameDelay = 1000 / FPS; // time between frames

	Uint32 frameStart; // huge int
	int frameTime;

	game = new Game;

	int total_ticks = 0; //used to keep score

	Sounds effects; // background music
	effects.background(1000); // passing in amount of loops

	game->init("Title here", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 832, 640, false);


	int powerTime = 0;
	int powerUp2Time = 0;
	unsigned int gameTime = 0;

	while (game->isRunning())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update(powerTime, powerUp2Time, gameTime);
		game->render(gameTime);

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) // then we need to delay frames
		{
			SDL_Delay(frameDelay - frameTime); // keeps constant character movement speed for diff computers
		}


		total_ticks += frameTime; //adding to total score

		//cout << "Total ticks: " << total_ticks << endl; // for me - prints out current total ticks
	}

	
	game->clean();

	return 0;

}
