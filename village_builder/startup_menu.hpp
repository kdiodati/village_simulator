#ifndef STARTUP_MENU_HPP_
#define STARTUP_MENU_HPP_

//#include "map.hpp"
#include <string>

// Called at the very start of the game. 
// Presents basic information (welcome, version, etc)
// Checks if the player is starting a new game or loading a save.
void Welcome();

// Sets initial parameters for the game, gets name of village, displays
// rules, and initializes the map.
std::string NewGame();

// Takes a save file input and recreates the game state using the information
// stored there (variables, objects, etc)
// Note: this is a lower priority implementation
//void LoadGame(ifstream &saveFile);

// Runs all the other functions
std::string RunStartupMenu();

// Checks if it is a common command (help, quit, etc)
// Returns a formatted sting for easier parsing of local commands.
std::string CheckInput(std::string in);

#endif
