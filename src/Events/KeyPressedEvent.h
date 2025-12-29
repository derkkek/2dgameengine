#pragma once
#include "../EventBus/Event.h"
#include <sdl/SDL.h>
class KeyPressedEvent : public Event
{
public:
	KeyPressedEvent(SDL_Keycode symbol)
		:symbol(symbol)
	{

	}
	~KeyPressedEvent() = default;
	SDL_Keycode symbol;

private:

};