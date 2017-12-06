#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include <string>
#include "ColliderComponent.h"

class ColliderComponent;

class KeyBoardController : public Component
{
public:
	TransformComponent *transform;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		

			if (Game::event.type == SDL_KEYDOWN)
			{
				switch (Game::event.key.keysym.sym)
				{
				case SDLK_a: // left
					transform->velocity.x = -2.0;
					break;
				case SDLK_d: // right
					transform->velocity.x = 2.0;
					break;
				default:
					break;
				}
			}

			if (Game::event.type == SDL_KEYUP)
			{
				switch (Game::event.key.keysym.sym)
				{
				case SDLK_a: // left
					transform->velocity.x = 0;
					break;
				case SDLK_d: // right
					transform->velocity.x = 0;
					break;
				default:
					break;
				}
			}
		
	}
};