#pragma once
#include "MapManager.hpp"
#include "TextureManager.hpp"
#include <SDL2/SDL.h>
#include <SDL_render.h>
class Player{
public:
	enum Direction{
		down=0,
		left,
		right,
		up
	};

	Player(SDL_Renderer*render,TextureManager * tm,MapManager *mg_);
	~Player();
	void handleEvent(const Uint8 *keystates);
	void update();
	void render();
	bool check_x(hitbox& a,hitbox& b);
	bool check_y(hitbox& a,hitbox& b);
	bool is_blocked(hitbox& a);

private:
	SDL_Rect srcRect_;
	SDL_Rect dstRect_;
	int x_;
	int y_;
	TextureManager* tm_;
	MapManager *mg_;
	hitbox trigerZone_;
	int frame_;
	int frameCounter_;
	Direction direction_=down;
	SDL_Renderer* renderer_;
	int speed;
};
