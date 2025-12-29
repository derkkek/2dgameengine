#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#include <sdl/SDL.h>
#include <string.h>
#include <iostream>
struct SpriteComponent
{
	std::string assetTextureID;
	int width;
	int height;
	int zIndex;
	bool isFixed;
	SDL_Rect sourceRect;

	SpriteComponent(std::string ID = "", int width = 5, int height = 5, int z_index = 0,bool fixed = false, int srcRectX = 0, int srcRectY = 0)
		:assetTextureID(ID), width(width), height(height), zIndex(z_index), isFixed(fixed), sourceRect({srcRectX, srcRectY, width, height})
	{

	}
};

#endif // !SPRITECOMPONENT_H
