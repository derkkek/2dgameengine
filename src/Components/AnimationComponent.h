#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <sdl/SDL.h>

struct AnimationComponent
{
	int numFrames;
	int currentFrame;
	int frameSpeedRate;
	bool isLoop;
	int startTime;

	AnimationComponent(int num_frames = 1, int FrameSpeedRate = 1, bool isLoop = true)
		:numFrames(num_frames), currentFrame(1), frameSpeedRate(FrameSpeedRate), isLoop(isLoop), startTime(SDL_GetTicks())
	{

	}
};
#endif // !ANIMATIONCOMPONENT_H
