#include "AssetStore.h"
#include <sdl/SDL_image.h>

AssetStore::~AssetStore()
{
	for (auto texture : textures)
	{
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

void AssetStore::ClearAssets()
{
	for (auto texture : textures)
	{
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& textureID, const std::string& filepath)
{
    SDL_Surface* surface = IMG_Load(filepath.c_str());
    if (!surface)
    {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        std::cerr << "Failed to create texture from surface: " << filepath << std::endl;
        std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    textures.emplace(textureID, texture);
}

SDL_Texture* AssetStore::GetTexture(const std::string& textureID)
{
	return textures[textureID];
}
