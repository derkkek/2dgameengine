#ifndef RENDERDEBUGSYSTEM_H
#define RENDERDEBUGSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class RenderDebugSystem : public System
{
public:
	RenderDebugSystem()
	{
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update(SDL_Renderer* renderer, SDL_Rect& camera)
	{
		for (auto entity : GetSystemEntities())
		{
			BoxColliderComponent& collider = entity.GetComponent<BoxColliderComponent>();
			TransformComponent transform = entity.GetComponent<TransformComponent>();
			SDL_Rect boundingBox = { transform.position.x - camera.x, transform.position.y - camera.y, collider.width * transform.scale.x + collider.offset.x , collider.height * transform.scale.y + collider.offset.y };
			
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			
			if (collider.isCollided)
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}

			SDL_RenderDrawRect(renderer, &boundingBox);
		}
	}
};

#endif