#pragma once
#include "../Components/BoxColliderComponent.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
class CollisionSystem : public System
{
public:
	CollisionSystem()
	{
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}

	~CollisionSystem() = default;


	void Update() 
	{
		auto entities = GetSystemEntities();
		for (auto i = entities.begin(); i != entities.end(); i++)
		{
			Entity a = *i;
			auto aTransform= a.GetComponent<TransformComponent>();
			auto &aCollider = a.GetComponent<BoxColliderComponent>();
			for (auto j = i + 1; j != entities.end(); j++)
			{
				Entity b = *j;
				
				auto bTransform = b.GetComponent<TransformComponent>();
				auto &bCollider = b.GetComponent<BoxColliderComponent>();

				bool collisionHappened = CheckAABBCollision(aTransform.position.x + aCollider.offset.x, aTransform.position.y + aCollider.offset.y, aCollider.width, aCollider.height,
					bTransform.position.x + bCollider.offset.x, bTransform.position.y + bCollider.offset.y, bCollider.width, bCollider.height);

				if (collisionHappened)
				{
					aCollider.isCollided = true;
					bCollider.isCollided = true;
				}

				else
				{
					aCollider.isCollided = false;
					bCollider.isCollided = false;
				}
			}
		}
		
	}

private:

	bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH)
	{
		return(aX < bX + bW && aX + aW > bX && aY < bY + bH && aY + aH > bY);
	}

};
