#ifndef ASSETSTORE_H
#define ASSETSTORE_H
#include <map>
#include <iostream>
#include <sdl/SDL.h>
class AssetStore
{
public:
	AssetStore() = default;
	~AssetStore();

	void ClearAssets();
	void AddTexture(SDL_Renderer* renderer, const std::string& textureID, const std::string& filepath);
	SDL_Texture* GetTexture(const std::string& textureID);

private:
	std::map<std::string, SDL_Texture*> textures;
};


#endif // !ASSETSTORE_H

