#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char * textureSheet, int x, int y)
{
	this->objectTexture = TextureManager::loadTexture(textureSheet);

	xPos = x;
	yPos = y;
}

GameObject::~GameObject()
{
}

void GameObject::update()
{
	xPos++;
	yPos++;

	this->srcR.h = 32;
	this->srcR.w = 32;
	srcR.x = 0;
	srcR.y = 0;

	destR.x = xPos;
	destR.y = yPos;
	destR.w = srcR.w * 2;
	destR.h = srcR.h * 2;
}

void GameObject::render()
{
	SDL_RenderCopy(Game::renderer, this->objectTexture, &this->srcR, &this->destR); // draws texture to screen
}
