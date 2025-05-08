#include "Game.hpp"
#include "MapManager.hpp"
#include "Player.hpp"
#include "TextureManager.hpp"
#include <iostream>
Game::Game(){}
Game::~Game(){}

void Game::init(const char* title,int width,int height){
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	    std::cerr << "SDL_Init failed :" << SDL_GetError() << std::endl;
	    return;
	  }
	window =
	      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			       width, height, SDL_WINDOW_SHOWN);
	if (!window) {
	    std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
	    return;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
	  std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
	  return;
	}
	textureManager_ = new TextureManager();
	textureManager_->initTexture(renderer);
	isRunning_ = true;
	mapManager_=new MapManager(renderer,textureManager_);
	
	player=new Player(renderer,textureManager_,mapManager_);
	
}
void Game::handleEvents() {
  SDL_Event event;
  SDL_PollEvent(&event);
  if (event.type == SDL_QUIT) {
    isRunning_ = false;
  }
  const Uint8 *keystates = SDL_GetKeyboardState(nullptr);
  player->handleEvent(keystates);
}

void Game::update() {
	player->update();
}


void Game::render() {
  SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
  SDL_RenderClear(renderer);
  update();
  mapManager_->render();
  player->render();
  SDL_RenderPresent(renderer);
}

void Game::clean() {
  delete mapManager_;
  delete textureManager_;
  delete player;
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
