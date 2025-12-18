#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H
#include<glm/glm.hpp>

struct RigidbodyComponent
{
	glm::vec2 velocity;

	RigidbodyComponent(glm::vec2 vel= glm::vec2(0.0, 0.0))
		:velocity(vel)
	{

	}
};

#endif // !RIGIDBODYCOMPONENT_H

