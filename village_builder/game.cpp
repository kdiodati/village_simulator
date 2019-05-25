#include "game.hpp"
#include "game_menu.hpp"
#include "map.hpp"
#include <string>

// Use this to find an error by line and file
#define CHECK_ERROR cout << "CHECK_ERROR on line " << __LINE__ << " in " << __FILE__ << "." << endl;

Game::Game() {
	//this->map;
	this->map.setName(RunStartupMenu());
	//this->map = map_;
	//this->map = RunStartupMenu();
	RunGameMenu(this->map);
}
