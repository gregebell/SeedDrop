#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SDL.h"
#include "SpriteComponent.h"

class TileComponent : public Component
{
public:
	TransformComponent *transform;
	SpriteComponent *sprite;

	SDL_Rect tileRect;
	int tileId;
	char* path;

	TileComponent() = default;

	TileComponent(int x, int y, int w, int h, int id)
	{
		tileRect.x = x;
		tileRect.y = y;
		tileRect.h = h;
		tileRect.w = w;

		tileId = id;

		switch (tileId)
		{
		case 0: 
			path = "assets/dirt1.png";
			break;
		case 1:
			path = "assets/dirt2.png";
			break;
		case 2: 
			path = "assets/dirt3.png";
			break;
		case 3:
			path = "assets/dirt_edge.png"; 
			break;
		case 4:
			path = "assets/dirt_edge_r.png";
			break;
		default:
			break;
		}
	}

	void init() override
	{
		entity->addComponent<TransformComponent>((float) tileRect.x, (float) tileRect.y, tileRect.w, tileRect.h, 1, 0);
		transform = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->getComponent<SpriteComponent>();
	}
};