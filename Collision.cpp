#include "Collision.h"
#include "ColliderComponent.h"


bool Collision::AABB(const SDL_Rect rectA, SDL_Rect rectB)
{
	if (
		rectA.x + rectA.w >= rectB.x &&
		rectB.x + rectB.w >= rectA.x &&
		rectA.y + rectA.h >= rectB.y &&
		rectB.y + rectB.h >= rectA.y
		)
	{
		return true;
	}

	return false;
}

bool Collision::AABB(const ColliderComponent& colliderA, const ColliderComponent& colliderB)
{
	if (&colliderB)
	{
		if (AABB(colliderA.collider, colliderB.collider))
		{
			return true;
		}
	}
	return false;
}