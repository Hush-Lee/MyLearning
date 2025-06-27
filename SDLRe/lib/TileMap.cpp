#include "TileMap.hpp"
#include "MapManager.hpp"
#include "TextureManager.hpp"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <vector>
TileMap::TileMap(SDL_Renderer*renderer,std::string name,TextureManager*tm):renderer_(renderer),name_(name),tm_(tm),frame_(0){
	std::ifstream in("assets/Map.json");
	nlohmann::json config;
	in>>config;
	auto map_config=config[name];
	int j=0;
	for(auto & col : map_config["Map"]){
		int h=0;
		std::vector<int> tmp;
		for(auto& i : col){
			tmp.push_back(i.get<int>());
			if(i.get<int>()==1){
				Forbidden_.push_back(hitbox{h*Scale*Size,j*Scale*Size,Scale*Size,Scale*Size});
			}
			if(i.get<int>()==3){
				tpPoint_.trigerZone=hitbox{h*Scale*Size,j*Scale*Size,Scale*Size,Scale*Size};
			}
			++h;
		}
		++j;
		map_.push_back(tmp);
	}
	tpPoint_.newX=map_config["TeleportPoint"]["x"].get<int>();
	tpPoint_.newY=map_config["TeleportPoint"]["y"].get<int>();
	tpPoint_.nextMap=map_config["TeleportPoint"]["nextMap"].get<std::string>();
	
}

void TileMap::render(){
	SDL_Rect srcRect,dstRect;
	srcRect.x=0;
	srcRect.y=0;
	srcRect.h=Scale*Size*10;
	srcRect.w=Scale*Size*10;
	dstRect.h=Scale*Size;
	dstRect.w=Scale*Size;
	for(int i=0;i<map_.size();++i){
		for(int j=0;j<map_[0].size();++j){
			dstRect.x=j*Scale*Size;
			dstRect.y=i*Scale*Size;
			switch (map_[i][j]) {
				case 0:
					SDL_RenderCopy(renderer_,tm_->mapElement_["grass"],&srcRect,&dstRect);
					break;
				case 1:
					SDL_RenderCopy(renderer_,tm_->mapElement_["wall"], &srcRect, &dstRect);
					break;
				case 3:
					SDL_RenderCopy(renderer_,tm_->mapElement_["grass"],&srcRect,&dstRect);
					renderPortal(dstRect);
					break;
			}
		}
	}
}
void TileMap::renderPortal(SDL_Rect& dstRect){
	SDL_Rect srcRect;
	srcRect.y=0;
	srcRect.x=(frame_/100+1)*Size*Scale;
	srcRect.w=Size*Scale;
	srcRect.h=Size*Scale;
	SDL_RenderCopy(renderer_,tm_->mapElement_["protal"],&srcRect, &dstRect);
	++frame_;
	if(frame_>8){
		frame_%=8;
	}
}

TeleportPoint* TileMap::tpPoint(){
	return &tpPoint_;
}
