#pragma once

#include <SDL2/SDL_render.h>
#include <string>
#include <unordered_map>
#include "TextureManager.hpp"
struct hitbox{
	int x;
	int y;
	int h;
	int w;
};
class TileMap;
struct TeleportPoint;

class MapManager{
public:
	MapManager(SDL_Renderer* renderer,TextureManager*tm);
	void Tp();
	void render();
	TileMap* currentMap(){
		return currentMap_;
	}
	TeleportPoint* tp(){
		return tp_;
	}

private:
	std::unordered_map<std::string,TileMap*> maps_;
	TileMap *currentMap_;
	TeleportPoint* tp_;
};
