#pragma once

#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent() = default;
	SpriteComponent(const char* path)
	{
		setTex(path);
	}

	SpriteComponent(const char* path, int nFrames, int speed)
	{
		animated = true;
		this->frames = nFrames;
		this->speed = speed;
		setTex(path);
	}

	~SpriteComponent()
	{
		SDL_DestroyTexture(texture1);
	}

	void setTex(const char* path)
	{
		texture1 = TextureManager::loadTexture(path);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		srcR.x = srcR.y = 0;
		srcR.w = transform->width;
		srcR.h = transform->height;
	}

	void update() override
	{
		if (animated)
		{
			srcR.x = srcR.w * static_cast <int> ((SDL_GetTicks() / speed) % frames);
		}

		destR.x = static_cast<int>(transform->position.x); // rect expects integers
		destR.y = static_cast<int>(transform->position.y);
		destR.w = transform->width * transform->scale;
		destR.h = transform->height * transform->scale;
	}

	void draw() override
	{
		TextureManager::draw(texture1, srcR, destR); // draw player to screen
	}
	void setSpeed(int speed)
	{
		this->speed = speed;
	}

	SDL_Rect srcR, destR;

private:
	TransformComponent *transform;
	SDL_Texture *texture1;
	//SDL_Texture *texture2;

	bool animated = false;
	int frames = 0;
	int speed = 100;
};