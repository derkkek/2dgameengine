#pragma once

struct HealthComponent
{
	int healthPercentage;

	HealthComponent(int health = 100)
		:healthPercentage(health)
	{

	}
};