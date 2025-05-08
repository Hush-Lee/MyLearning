#include "MapManager.hpp"
#include "TextureManager.hpp"
#include "TileMap.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

MapManager::MapManager(SDL_Renderer* renderer,TextureManager* tm){
	nlohmann::json config;
	std::fstream in("../src/Map.json");
	in>>config;
	for(auto& m: config["Maps"]){
		maps_.insert({m.get<std::string>(),new TileMap(renderer,m.get<std::string>(),tm)});
	}
	currentMap_=maps_["Map1"];
	tp_=currentMap_->tpPoint();
}

void MapManager::render(){
	currentMap_->render();
}

void MapManager::Tp(){
	currentMap_ =maps_[tp_->nextMap];
	tp_=currentMap_->tpPoint();
}
