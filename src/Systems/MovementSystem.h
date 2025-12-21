#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include <spdlog/spdlog.h>

class MovementSystem :public System
{
public:
	MovementSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidbodyComponent>();
	}
	~MovementSystem() = default;

	void Update(double deltaTime)
	{
		for (auto entity : GetSystemEntities())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rigidbody = entity.GetComponent<RigidbodyComponent>();

			transform.position.x += rigidbody.velocity.x * deltaTime;
			transform.position.y += rigidbody.velocity.y * deltaTime;
			//spdlog::info("position entity x: " + std::to_string(transform.position.x));
		}
	}
private:

};


#endif // !MovementSystem

