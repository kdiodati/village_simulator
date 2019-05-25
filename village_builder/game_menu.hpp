#ifndef GAME_MENU_HPP_
#define GAME_MENU_HPP_

#include "map.hpp"
#include <string>

// Checks if it is a global command (help, quit, etc)
// Returns a formatted string for easier parsing of local commands.
std::string CheckInput2(std::string in);

void RunGameMenu(Map map);

#endif
