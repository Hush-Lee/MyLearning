#include "TextureManager.hpp"
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
void checkJson(nlohmann::json j){
	if(j.empty()){
		std::cerr<<"empty json,break";
	}
}
SDL_Texture *TextureManager::LoadTexture(const std::string &file,
                                         SDL_Renderer *renderer) {
  SDL_Surface *tempSurface = IMG_Load(file.c_str());
  if (!tempSurface) {
    SDL_Log("Failed to load image : %s", SDL_GetError());
    std::cerr << "Falied to load image :" << SDL_GetError() << std::endl;
    return nullptr;
  }
  SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
  SDL_FreeSurface(tempSurface);
  return tex;
}
void TextureManager::initTexture(SDL_Renderer *renderer) {
  std::ifstream in("assets/Map.json");
  if (!in.is_open()) {
    std::cerr << "Open file Failed\n";
    return;
  }
  nlohmann::json config;
  in >> config;
  checkJson(config);
  for (auto &p : config["Part"]) {
    mapElement_.insert({p.get<std::string>(),LoadTexture(config["Texture"][p.get<std::string>()].get<std::string>(),renderer)});
  }
  in.close();
  std::ifstream fin("assets/Role.json");
  if (!fin.is_open()) {
    std::cerr << "open file Failed";
    return;
  }
  config.clear();
  fin >> config;
  for (auto &p : config["Part"]) {
    checkJson(p);
    auto key = p.get<std::string>();
    roleElement_.insert(
        {key, LoadTexture(config["Texture"][p.get<std::string>()].get<std::string>(), renderer)});
  }
  fin.close();
}
