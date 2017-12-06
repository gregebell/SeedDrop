#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#include <string.h>

#include "MusicPlayer.h"


class ColliderComponent;

enum groupLabels : std::size_t
{
	groupScreens,
	groupScreen2,
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders,
	groupScore
	

};

using std::cout;
using std::endl;

class Game 
{
public:
	Game();
	~Game();

	void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen); // initializes game window
	void update(int &powerUpTime, int &powerUp2Time, unsigned int &gameTime); // updates object locations
	void render(unsigned int &gameTime); // puts changes on screen
	void clean();
	void handleEvents();

	bool isRunning();
	void setRunning(bool running);
	bool getRunning() { return this->running;  }

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	static void addTile(int id, int x, int y);

	void createRoot(int powerTime, unsigned int gameTime);
	void createRoot(unsigned int gameTime,double velchange, int freq1, int freq2);
	void createMiddleRoot(int velocity);
	void createLeftRoot(int velocity, unsigned int gametime);
	void createRightRoot(int velocity, unsigned int gametime);
	
	void printScore(int gametime);
	void score(int gametime);
//	void (int lower, int higher, double module, double velocity)
	void createPowerUp(int pos, int pos2);
	void createRock(int pos, int pos2);
	void createWorm(int pos, int pos2);
	void createSun(int pos, int pos2);

	//my stuff for score
	int calculateScore(int totalTicks);

	
private:
	int count = 0;
	std::string numb1;
	std::string numb2;
	std::string numb3;
	bool playerDead = false;
	bool running;
	SDL_Window *window;
	Sounds actionEffects;
	
};

#endif
