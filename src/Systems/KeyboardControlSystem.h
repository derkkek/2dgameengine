#pragma once
#include "../EventBus/EventBus.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidbodyComponent.h"
class KeyboardControlSystem : public System
{
public:
	KeyboardControlSystem()
	{
		RequireComponent<KeyboardControlledComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<RigidbodyComponent>();
	}
	~KeyboardControlSystem() = default;

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyboard);
	}

	void OnKeyboard(KeyPressedEvent& symbol)
	{
		for (auto entity : GetSystemEntities())
		{
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& rigidBody = entity.GetComponent<RigidbodyComponent>();
			auto keyboardControl = entity.GetComponent<KeyboardControlledComponent>();

			switch (symbol.symbol)
			{
			case(SDLK_UP):
				sprite.sourceRect.y = sprite.height * 0;
				rigidBody.velocity = keyboardControl.upVelocity;
				break;
			case(SDLK_RIGHT):
				sprite.sourceRect.y = sprite.height * 1;
				rigidBody.velocity = keyboardControl.rightVelocity;
				break;
			case(SDLK_DOWN):
				sprite.sourceRect.y = sprite.height * 2;
				rigidBody.velocity = keyboardControl.downVelocity;
				break;
			case(SDLK_LEFT):
				sprite.sourceRect.y = sprite.height * 3;
				rigidBody.velocity = keyboardControl.leftVelocity;
				break;
			default:
				break;
			}
		}
	}
private:

};