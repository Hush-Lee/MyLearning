#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <string>
#include <unordered_map>
class TextureManager{
public:
	SDL_Texture* LoadTexture(const std::string& path,SDL_Renderer* renderer);
	void initTexture(SDL_Renderer *renderer);
	std::unordered_map<std::string,SDL_Texture*> mapElement_;
	std::unordered_map<std::string,SDL_Texture*> roleElement_;
	SDL_Texture* getTexture(int choice,std::string& element);
};


