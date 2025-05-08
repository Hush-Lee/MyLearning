#include "Player.hpp"
#include "MapManager.hpp"
#include "TextureManager.hpp"
#include "TileMap.hpp"
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_scancode.h>
#include <nlohmann/json.hpp>
#include <SDL_render.h>
#include <fstream>
extern const int Size;
extern const int Scale;
Player::Player(SDL_Renderer* renderer,TextureManager* tm,MapManager* mg):renderer_(renderer),tm_(tm),frame_(0),frameCounter_(0),mg_(mg){
	nlohmann::json config;
	std::fstream in("../src/Role.json");
	in>>config;
	config=config["locate"];
	x_=config["x"].get<int>()*Scale*Size;
	y_=config["y"].get<int>()*Scale*Size;
	srcRect_={0,0,Size,Size};
	dstRect_={x_,y_,Size*Scale,Size*Scale};
	speed=config["speed"];
	trigerZone_=mg_->currentMap()->tpPoint()->trigerZone;
}
Player::~Player(){

}

void Player::render(){
	for(auto part:tm_->roleElement_){
		SDL_RenderCopy(renderer_, part.second, &srcRect_, &dstRect_);
	}
}
bool Player::check_x(hitbox& a, hitbox& b) {
    return a.x < b.x + b.w && a.x + a.w > b.x;
}

bool Player::check_y(hitbox& a, hitbox& b) {
    return a.y < b.y + b.h && a.y + a.h > b.y;
}


bool Player::is_blocked(hitbox& player){
	for(auto & block:mg_->currentMap()->Forbidden()){
		if(check_x(player,block)&&check_y(player,block)){
			return true;
		}
	}
	return false;
}
void Player::handleEvent(const Uint8* keystates){
	int x=x_;
	int y=y_;
	if(keystates[SDL_SCANCODE_DOWN]||keystates[SDL_SCANCODE_S]){
		y_+=speed;
		direction_=down;
	}else if (keystates[SDL_SCANCODE_UP]||keystates[SDL_SCANCODE_W]) {
		y_-=speed;
		direction_=up;
	}else if(keystates[SDL_SCANCODE_RIGHT]||keystates[SDL_SCANCODE_D]){
		x_+=speed;
		direction_=right;
	}else if(keystates[SDL_SCANCODE_LEFT]||keystates[SDL_SCANCODE_A]){
		x_-=speed;
		direction_=left;
	}else{
		frame_=1;
		return;
	}
	hitbox player{x_,y_,Scale*Size,Scale*Size};
	if(is_blocked(player)){
		x_=x;
		y_=y;
	}
	
	++frameCounter_;
	if(frameCounter_>=10){
		frame_=(frame_+1)%3;
		frameCounter_=0;
	}
	if(check_x(player,trigerZone_)&&check_y(player,trigerZone_)){
		x_=mg_->tp()->newX*Scale*Size;
		y_=mg_->tp()->newY*Scale*Size;
		mg_->Tp();
		trigerZone_=mg_->tp()->trigerZone;
	}

}

void Player::update(){
	srcRect_.x=frame_*Size;
	srcRect_.y=direction_*Size;
	dstRect_.x=x_;
	dstRect_.y=y_;
}
