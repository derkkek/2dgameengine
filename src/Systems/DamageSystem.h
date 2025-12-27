#pragma once
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Components/BoxColliderComponent.h"

class DamageSystem : public System
{
public:
	DamageSystem()
	{
		RequireComponent<BoxColliderComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
	}
	
	void OnCollision(CollisionEvent& event)
	{
		event.a.Kill();
		event.b.Kill();
	}

	void Update()
	{

	}
private:
};