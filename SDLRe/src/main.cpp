#include "Game.hpp"
int main(){
	Game game;
	game.init("2D RPG",1020,690);
	while (game.running()) {
		game.handleEvents();
		game.render();
	}
	game.clean();
	
}
