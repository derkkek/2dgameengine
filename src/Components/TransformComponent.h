#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include <glm/glm.hpp>

struct TransformComponent
{
	TransformComponent(glm::vec2 pos = glm::vec2(0.0f,0.0f), glm::vec2 scale=glm::vec2(1,1), double rotation = 0)
		:position(pos), scale(scale), rotation(rotation)
	{

	}

	glm::vec2 position;
	glm::vec2 scale;
	double rotation;
};

#endif // !TRANSFORMCOMPONENT_H

