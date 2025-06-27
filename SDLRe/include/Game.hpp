#pragma once
#include "MapManager.hpp"
#include "Player.hpp"
class Game{
public:
	Game();
	~Game();
	void init(const char* title,int width, int height);
	void handleEvents();
	void render();
	void update();
	void clean();
	bool running()const {return isRunning_;}

private:
	MapManager* mapManager_;
	bool isRunning_;
	Player*player=nullptr;
	SDL_Window* window=nullptr;
	SDL_Renderer* renderer=nullptr;
	TextureManager *textureManager_;
};
