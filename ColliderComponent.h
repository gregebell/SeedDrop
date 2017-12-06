#pragma once

#include <string>
#include <SDL.h>
#include "Components.h"

class ColliderComponent : public Component
{
public:
	ColliderComponent(std::string t) { tag = t;  }

	SDL_Rect collider;

	std::string tag; // name of the collidable object

	TransformComponent *transform;
	
	void init() override
	{
		if (!entity->hasComponent<TransformComponent>()) // makes sure entity has a transform component first
		{
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();

		Game::colliders.push_back(this); // add this component to array of colliders
	}

	void update() override
	{
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);

		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
	}
};