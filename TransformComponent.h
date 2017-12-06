#pragma once

#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;
	float scale = 1;
	
	int speed = 3;

	TransformComponent()
	{
		position.zero();

		velocity.zero();
	}

	TransformComponent(float x, float y, int h, int w, float scale, float y_velocity)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		this->scale = scale;
		velocity.y = y_velocity;
	}

	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;

		if (position.y < 0) // root is off screen
		{
			this->entity->destroy();
		}
	}
};