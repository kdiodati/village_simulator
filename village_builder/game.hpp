#ifndef GAME_HPP_
#define GAME_HPP_

#include "startup_menu.hpp"
#include "game_menu.hpp"
#include "resource.hpp"
#include "villagers.hpp"
#include "map.hpp"
#include <string>

// Consolidate menus here for simple call to main.cpp


class Game {
	public:
		Game();
		
		//getMap();
		
		//setMap();
		// MENU    
	private:
		Map map;
};
#endif
