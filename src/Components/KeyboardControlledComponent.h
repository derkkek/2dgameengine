#pragma once
#include <glm/glm.hpp>



struct KeyboardControlledComponent
{
	glm::vec2 upVelocity;
	glm::vec2 rightVelocity;
	glm::vec2 downVelocity;
	glm::vec2 leftVelocity;

	KeyboardControlledComponent(glm::vec2 up_vel = glm::vec2(0), glm::vec2 right_vel = glm::vec2(0), glm::vec2 down_vel = glm::vec2(0), glm::vec2 left_vel = glm::vec2(0))
		:upVelocity(up_vel), rightVelocity(right_vel), downVelocity(down_vel), leftVelocity(left_vel)
	{

	}

};