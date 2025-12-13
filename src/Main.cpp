// Fix for SDL_ttf deprecated attribute syntax issue with MSVC
#define SDL_DEPRECATED

#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <sdl/SDL_ttf.h>
#include <sdl/SDL_mixer.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <sol/sol.hpp>

int main(int argc, char* argv[])
{
	glm::vec2 vel;
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	SDL_Init(SDL_INIT_EVERYTHING);
	return 0;
}