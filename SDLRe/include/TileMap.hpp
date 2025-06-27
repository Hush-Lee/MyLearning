#pragma once
#include <SDL2/SDL.h>
#include "MapManager.hpp"
#include "TextureManager.hpp"
#include <SDL2/SDL_render.h>
#include <SDL_rect.h>
#include <cstddef>
#include <functional>
#include <string>
#include <utility>
#include <vector>

struct TeleportPoint{
	int newX;
	int newY;
	hitbox trigerZone;
	std::string nextMap;
};
struct pair_hash{
	size_t operator()(const std::pair<int,int>&p)const{
		return std::hash<int>()(p.first)^std::hash<int>()(p.second);
	}
};
static constexpr int Scale=2;
static constexpr int Size=32;

class TileMap{
public:
	TileMap(SDL_Renderer* renderer,std::string name,TextureManager*t);
	~TileMap();
	TeleportPoint* tpPoint();
	void render();
	std::vector<hitbox>Forbidden(){
		return Forbidden_;
	}
	void renderPortal(SDL_Rect &dstRect);
private:
	SDL_Renderer *renderer_;
	std::string name_;
	std::vector<std::vector<int>> map_;
	TeleportPoint tpPoint_;
	TextureManager * tm_;
	std::vector<hitbox>Forbidden_;
	int frame_;
};
