#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <sdl/SDL.h>
#include <glm/glm.hpp>

struct BoxColliderComponent
{
	int width;
	int height;
	glm::vec2 offset;
	bool isCollided;

	BoxColliderComponent(int Width = 0, int Height = 0, glm::vec2 Offset = glm::vec2(0))
		:width(Width), height(Height), offset(Offset)
	{
	}
};
#endif // !BOXCOLLIDERCOMPONENT_H
