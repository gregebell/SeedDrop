#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include "Game.h"

class GameObject
{
public:
	GameObject(const char* textureSheet, int x, int y);
	~GameObject();

	void update();
	void render();

private:
	int xPos; // object location on screen
	int yPos;

	SDL_Texture* objectTexture;
	SDL_Rect srcR, destR;
};

#endif