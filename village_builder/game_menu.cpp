#include "game_menu.hpp"
#include "map.hpp"
#include "events.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>

// Use this to find an error by line and file
#define CHECK_ERROR cout << "CHECK_ERROR on line " << __LINE__ << " in " << __FILE__ << "." << endl;

#define BORDER "================================================================================"
#define SPLITTER cout << "--------------------------------------------------------------------------------" << endl;
#define VERSION "0.3"

using namespace std;

// Checks the input string for global commands. Takes in a string 'in'. Returns a formatted input string for easier parsing
// of local commands.
string CheckInput2(string in) {
	// Changes the input string to lowercase for easy parsing.
	transform(in.begin(), in.end(), in.begin(), ::tolower);
	if (in == "help" || in == "commands" || in == "c") {
		cout << BORDER << endl;

		cout << "                          COMMANDS                          " << endl;
		// Commands that will always appear at the top.
		cout << "'Commands' or 'c' or 'help' - Displays a list of commands." << endl;
		cout << "'Guide' or 'g' - Displays a guide to playing" << endl;
		cout << "'Quit' or 'q' - Exits the program." << endl;
		cout << endl;

		cout << "'Map' or 'm' - Displays the map." << endl;
		cout << "'Terrain' or 't' - Displays a map of the terrain." << endl;
		cout << "'Resources' or 'r' - Displays resource information." << endl;
		cout << endl;

		cout << "'Upgrade' or 'u' - Upgrade an existing building." << endl;
		cout << "'Build' or 'b' - Build a new building." << endl;
		cout << "'Demolish' or 'd' - Demolish a building and fire its workers." << endl;
		cout << endl;

		cout << "'Villagers' or 'v' - Lists villagers and their information." << endl;
		cout << "'Hire' or 'h' - Hire a villager for a job.'" << endl;
		cout << "'Fire' or 'f' - Fire a villager from a job.'" << endl;
		cout << endl;

		cout << "'Advance' or 'a' - Moves time forward one day." << endl;
		cout << "'Jump' or 'j' - Jumps forward in time multiple days." << endl;
		//cout << "'Save' - Saves the game (Not implemented)." << endl;

		cout << BORDER << endl;
		// Reassign the string to be returned to return so that it doesn't print invalid command
		// once it is returned.
		in = "continue";
	}
	else if (in == "guide" || in == "g") {
		cout << "                                                  GUIDE                                                  " << endl;
		cout << "The goal of this game is to keep your village alive for as long as you can. You start with five villagers" << endl;
		cout << "who aren't assigned to any jobs. The map, which you can display with 'm' on the menu will be blank at the" << endl;
		cout << "start. You can only build certain buildings on specific terrains (ie. farm->plains, water source->lake)." << endl;
		cout << "Each building produces some kind of resource that you can use to build more buildings or upgrade existing" << endl;
		cout << "ones. A good place to start is by building a farm and hiring some of your villagers to work it. Make sure" << endl;
		cout << "you check your resources with 'r' regularly so you know if you can afford the changes. When you advance one" << endl;
		cout << "day or multiple days with 'j', there's a chance of an event occurring which may help or harm your village!" << endl;
		in = "continue";
	}
	else if (in == "quit" || in == "q") {
		cout << "Quitting Game..." << endl;
		exit(0);
	}
	// Return the parsed string
	return in;
}

// Runs the game menu. Takes in a Map that it will use for the rest of the game. We don't need to return it because
// it is called by startup_menu.cpp which already has the object saved.
void RunGameMenu(Map map) {
	// Holds the input text from the player.
	string buffer;
	Village v;
	mt19937 rng;
	// Set the random number generator seed
	rng.seed(random_device()());
	// Prints the commands list on startup
	buffer = "help";
	CheckInput2(buffer);
	buffer = "";
	int day = 1;
	SPLITTER
	cout << "Day: " << day << endl;
	SPLITTER
	// Primary game loop that runs until the player quits the game.
	while(true) {
		//cin.ignore(10000,'\n');
		cout << "Menu >";
		v = map.getVillage();
		// Get user input and put it in buffer.
		cin >> buffer;
		SPLITTER
		// Sends the user input to the CheckInput function which checks for common global commands 
		// (i.e. 'help', 'quit', etc). It also formats the string so that we get it back in all
		// lowercase to check for the local commands.
		buffer = CheckInput2(buffer);
		// Prints the map for the player
		if (v.getVector().size() == 0) {
			cout << endl << "GAME OVER!" << endl <<  "Your Village: " << map.getName() << " survived: " << day << " days." << endl;
			exit(2); //2 for gameover
		}
		else if (buffer == "terrain" || buffer == "t") {
			map.view();
		}
		// Prints a detailed map for the player
		else if (buffer == "map" || buffer == "m") {
			map.detailedView();
		}
		// Prints the resources information for the player
		else if (buffer == "resources" || buffer == "r") {
			map.getResources();
		}
		// Advances the game by one day, 
		else if (buffer == "advance" || buffer == "a") {
			// Variable to hold the player's decision on whether or not they want to continue
			int yorn = 0;
			// If there are jobless villagers...
			if (v.getJobless() != -1) {
				while (true) {
					string yesno;
					cout << "You have villagers that aren't working. Are you sure you want to advance?" << endl;
					cout << "Y/N >";
					// Get player response on if they want to continue even though they have jobless villagers.
					cin >> yesno;
					yesno = CheckInput2(yesno);
					// If the player wants to continue...
					if (yesno == "continue" || yesno == "yes" || yesno == "y") {
						// Reassign for later checking
						yorn = 0;
						break;
					}
					// If the player doesn't want to continue...
					else if (yesno == "no" || yesno == "n") {
						// Reassign for later checking
						yorn = 1;
						break;
					}
					// If the player issues an invalid command...
					else {
						cout << "Invalid Command" << endl;
						continue;
					}
				}
			}
			// If the player doesn't want to continue and potentially assign their jobless villagers...
			if (yorn == 1) {
				continue;
			}

			// Calculates gained resources when a day passes
			map.collect();
			// Calculates consumed resources
			map.consume();
			// Checks for villagers that were killed by hunger or thirst
			map.deathCheck();
			// Refreshes the map
			v = map.getVillage();

			if (v.getVector().size() == 0) {
				cout << endl << "GAME OVER!!!!" << endl << "Your Village: " << map.getName() << " survived: " << day << " days." << endl;
				exit(2);
			}
			++day;
			SPLITTER
			cout << "Day: " << day << endl;
			SPLITTER

			v = map.getVillage();
			// Get a random number to check for events
			uniform_int_distribution<int> d(1,35);
			int chance = d(rng);
			// Storm
			if (chance == 1) {
				int i = doStorm(map.getStone()->getCount());
				map.addStone(0 - i);
			}
			// Fire
			else if (chance == 2) {
				int i = doFire(map.getLogs()->getCount());
				map.addLogs(0 - i);
			}
			// Harvest
			else if (chance == 3) {
				int i = doHarvest(map.getFood()->getCount());
				map.addFood(i);
			}
			// Rain
			else if (chance == 4) {
				int i = doRain(map.getWater()->getCount());
				map.addWater(i);
			}
			// Infestation
			else if (chance == 5) {
				int i = doInfestation(map.getFood()->getCount());
				map.addFood(0 - i);
			}
			// Contamination
			else if (chance == 6) {
				int i = doContamination(map.getWater()->getCount());
				map.addWater(0 - i);
			}
			// Animal Attack
			else if (chance == 7) {
				int f = map.getFood()->getCount();
				int i = doAnimal(rng);
				if (i == -1) {
					v.killRand(); 
				}
				else {
					map.addFood(i);
				}
			}
			// Raid
			else if (chance == 8) {
				int s = map.getStone()->getCount();
				int i = doRaid(rng);
				if (i <= -1) {
					v.killRand(); 
					map.addStone(i);
				}
				else {
					i = 0 - i;
					map.addStone(i);
				}
			}
			// Winter
			else if (chance == 9) {
				int i = doWinter(map.getFood()->getCount());
				map.addFood(0 - i);
			}
			// New Villager(s)
			else if (chance >= 10 && chance <= 15) {
				int i = doSettler(rng);
				if (i != -1) {
					for (int j=0; j<i; j++) {
						v.addCitizen();
					}
				}
			}
			// No Event
			else {
				cout << endl << "No event occured. " << endl;
			}
			// Refresh the village in the map
			map.setVillage(v);
			// Check for dead villagers
			map.deathCheck(); 
			// Advance the day
		}
		// Multiple Day Advance
		else if (buffer == "jump" || buffer == "j") {
			// move forward in time
			int num = 0;
			cout << "Days >";
			cin >> num;
			if (cin.fail()) {
				cin.clear();
				num = -1;
			}
			if (num == 0) {
				continue;
			}
			if (num == -1) {
				cout << "Incorrect Input!" << endl;
				continue;
			}
			// Variable to hold the player's decision on whether or not they want to continue
			int yorn = 0;
			// If there are jobless villagers...
			if (v.getJobless() != -1) {
				while (true) {
					string yesno;
					cout << "You have villagers that aren't working. Are you sure you want to advance?" << endl;
					cout << "Y/N >";
					// Get player response on if they want to continue even though they have jobless villagers.
					cin >> yesno;
					yesno = CheckInput2(yesno);
					// If the player wants to continue...
					if (yesno == "continue" || yesno == "yes" || yesno == "y") {
						// Reassign for later checking
						yorn = 0;
						break;
					}
					// If the player doesn't want to continue...
					else if (yesno == "no" || yesno == "n") {
						// Reassign for later checking
						yorn = 1;
						break;
					}
					// If the player issues an invalid command...
					else {
						cout << "Invalid Command" << endl;
						continue;
					}
				}
			}
			// Loop through days
			for (int z = 1; z <= num; z++) {
				// Calculates gained resources when a day passes
				map.collect();
				// Calculates consumed resources
				map.consume();
				// Checks for villagers that were killed by hunger or thirst
				map.deathCheck();
				// Refreshes the map
				v = map.getVillage();
				
				if (v.getVector().size() == 0) {
					cout << endl << "GAME OVER! Your Village: " << map.getName() << " survived: " << day << "days." << endl;
					exit(2); //2 for gameover
				}

				// Print what day it is
				++day;
				cout << "-----" << endl;
				cout << "Day: " << day << " (" << z << ")" << endl;
				// Get a random number to check for events
				uniform_int_distribution<int> d(1,35);
				int chance = d(rng);
				// Storm
				if (chance == 1) {
					int i = doStorm(map.getStone()->getCount());
					map.addStone(0 - i);
				}
				// Fire
				else if (chance == 2) {
					int i = doFire(map.getLogs()->getCount());
					map.addLogs(0 - i);
				}
				// Harvest
				else if (chance == 3) {
					int i = doHarvest(map.getFood()->getCount());
					map.addFood(i);
				}
				// Rain
				else if (chance == 4) {
					int i = doRain(map.getWater()->getCount());
					map.addWater(i);
				}
				// Infestation
				else if (chance == 5) {
					int i = doInfestation(map.getFood()->getCount());
					map.addFood(0 - i);
				}
				// Contamination
				else if (chance == 6) {
					int i = doContamination(map.getWater()->getCount());
					map.addWater(0 - i);
				}
				// Animal Attack
				else if (chance == 7) {
					int f = map.getFood()->getCount();
					int i = doAnimal(rng);
					if (i == -1) {
						v.killRand(); 
					}
					else {
						map.addFood(i);
					}
				}
				// Raid
				else if (chance == 8) {
					int s = map.getStone()->getCount();
					int i = doRaid(rng);
					if (i <= -1) {
						v.killRand(); 
						map.addStone(i);
					}
					else {
						i = 0 - i;
						map.addStone(i);
					}
				}
				// Winter
				else if (chance == 9) {
					int i = doWinter(map.getFood()->getCount());
					map.addFood(0 - i);
				}
				// New Villager(s)
				else if (chance >= 10 && chance <= 15) {
					int i = doSettler(rng);
					if (i != -1) {
						for (int j=0; j<i; j++) {
							v.addCitizen();
						}
					}
				}
				// No Event
				else {
					cout << endl << "No event occured. " << endl;
				}
				// Refresh the village in the map
				map.setVillage(v);
				// Check for dead villagers
				map.deathCheck(); 
			}
		}
		// Don't need to worry about this yet unless we get time to implement saving.
		else if (buffer == "save") {
			// Planned on creating a way to make a save file, but ran out of time.
			cout << "Save fuctionality not implemented yet." << endl;
		}
		else if (buffer == "continue") {
			continue;
		}
		else if (buffer == "set") {
			string name_;
			int id;
			string first;
			string last;
			cout << "ID >";
			cin >> id;
			bool inside = false;
			// Check if id entered is in range of village.size()
			if (id > 0 && id <= v.getVector().size()) {
				inside = true;
				id--;
			}
			// cin >> last;
			// name_ = first + " " + last;
			// transform(name_.begin(), name_.end(), name_.begin(), ::tolower);
			// int i = v.checkName(name_);
			if (inside) {
				cout << "Job >";
				cin >> buffer;
				buffer = CheckInput2(buffer);
				if (buffer == "farmer") {
					v.makeFarmer(id);
				}
				else if (buffer == "fisher") {
					v.makeFisher(id);
				}
				else if (buffer == "lumberjack") {
					v.makeLumberjack(id);
				}
				else if (buffer == "miner") { 
					v.makeMiner(id);
				}
				/*
				else if (buffer == "builder") { //not Implemented
					v.makeBuilder(id);
				}
				*/
				else {
					cout << "Invalid job: " << buffer << endl;
				}
			}
			else {
				cout << "Invalid ID: " << id << endl;
			}
			map.setVillage(v);
		}
		// Checks if the user wants to build a building.
		// If they do, print out what they can build at those coordinates.
		// Ask what they want to build from the list and build it.
		else if (buffer == "build" || buffer == "b") {
			// Tile to build on [0, 9]
			int x;
			map.detailedView();
			cout << "Tile >";
			cin >> x;
			if (cin.fail()) {
				cin.clear();
				x = -10;
			}
			if (x <= -1 || x >= 9) {
				cout << "Incorrect Input!" << endl;
			}
			else {
				auto poss = map.availableBuildings(x);
				int y;
				cout << "Building >";
				cin >> y;
				if (cin.fail()) {
					cin.clear();
					y = -10;
				}
				if ( y != 0) {
					cout << "Incorrect Input!" << endl;
				}
				else {
					Building b = Building(poss[y]);
					map.build(map.getTiles()[x], b);
				}
			}
		}
		// Checks if the user wants to upgrade a building.
		else if (buffer == "upgrade" || buffer == "u") {
			int x;
			map.detailedView();
			cout << "Tile >";
			cin >> x;
			if (cin.fail()) {
				cin.clear();
				x = -10;
			}
			if (x <= -1 || x >=9) {
				cout << "Incorrect Input!" << endl;
			}
			else {
				auto build = map.getBuildings()[map.getTiles()[x]];
				map.listBuildings(x);
				int y;
				cout << "Building >";
				cin >> y;
				if (cin.fail()) {
					cin.clear();
					y = -10;
				}
				if ( y != 0) {
					cout << "Incorrect Input!" << endl;
				}
				else if (build.size() == 0) {
					cout << "There are no buildings in that tile." << endl;
				}
				else {
					auto tile = map.getTiles()[x];
					auto bldg = map.getBuildings()[tile][y];
					map.upgrade(tile, bldg);
				}
			}
		}
		// Checks if player wants to demolish a building.
		else if (buffer == "demolish" || buffer == "d") {
			// Tile to demolish on [0, 9]
			int x;
			map.detailedView();
			cout << "Tile >";
			cin >> x;
			if (cin.fail()) {
				cin.clear();
				x = -10;
			}
			if (x <= -1 || x >= 9) {
				cout << "Incorrect Input!" << endl;
			}
			else {
				auto build = map.getBuildings()[map.getTiles()[x]];
				map.listBuildings(x);
				int y;
				cout << "Building >";
				cin >> y;
				if (cin.fail()) {
					cin.clear();
					y = -10;
				}
				if (y != 0) {
					cout << "Incorrect Input!" << endl;
				}
				
				else if (build.size() == 0) {
					cout << "There are no buildings in that tile." << endl;
				}
				
				else {
					auto tile = map.getTiles()[x];
					map.demolish(tile, map.getBuildings()[tile][y]);
				}
			}
        	}
		// Assigns a player to a building and sets their job to work in said building.
        	else if (buffer == "hire" || buffer == "h") {
				int x;
				map.detailedView();
				cout << "Tile >";
				cin >> x;
			if (cin.fail()) { //if not an int set x to be an int outside of the range
				cin.clear();
				x = -10;
			}
			if (x <= -1 || x >= 9) {
				cout << "Incorrect Input!" << endl;
			}
			else {
				auto build = map.getBuildings()[map.getTiles()[x]];
				int y;
				map.listBuildings(x);
				cout << "Building >";
				cin >> y;
				if (cin.fail()) { //if not an int set y to be an int outside of the range
					cin.clear();
					y = -10;
				}
				if (y != 0) {
					cout << "Incorrect Input!" << endl;
				}
				else if (build.size() == 0) {
					cout << "There are no buildings in that tile." << endl;
				}
				else {
					map.assign(x, build[y]);
				}
			}
        	}
		// Unassigns a player from a building and makes their job 'none'.
        else if (buffer == "fire" || buffer == "f") {
		v.printVillage();
        	int x;
			cout << "ID >";
			cin >> x;
			if (cin.fail()) {
				cout << "Incorrect Input!" << endl;
				x = -1;
			}
			else if (x >=0 && x < v.getVector().size()) {
				map.unassign(x);
			}
		}
		else if (buffer == "villagers" || buffer == "v") {
			v.printVillage();
		}
		else if (buffer == "jobs") {
			v.printJobs();
		}
			/*
			else if (buffer == "job") {
				cin >> buffer;
				buffer = CheckInput2(buffer);
				if (buffer == "list") {
					cout << "Farmer: provides food for the village" << endl;
					cout << "Fisher: provides food for the village" << endl;
					cout << "LumberJack: provides logs" << endl;
					cout << "Miner: provides stone, works in a quarry" << endl;
					cout << "Builder: << builds buildings for the village" << endl;
					//cout << "-- full implementation coming --" << endl; //TODO
					//cout << "farmer, fisher, lumberjack, miner, builder" << endl;
				}
				else {
					cout << "Invalid command: enter 'Help for a list of commands." << endl;
				}
			}
			*/
		// Dev cheats, useful in testing functions, adds
        else if (buffer == "cheat") {
           		cout << "CHEAT -- ADDING RESOURCES" << endl;
           		map.addLogs(100);
           		map.addFood(100);
           		map.addWater(100);
           		map.addStone(100);
        }
		else if (buffer == "ch") {
			cout << "CHEAT -- ADDING VILLAGER" << endl;
			Village v = map.getVillage();
			v.addCitizen();
			map.setVillage(v);
		}
		// The command wasn't recognized so print this error and return to the top of the loop.
		else {
			cout << "Invalid command: enter 'Help' for a list of commands." << endl;
		}
		// Reduce CPU load by waiting for a second between loops.
		SPLITTER
		
		// Clears up remaining cin that isn't used
		cin.clear();
		cin.ignore(10000,'\n');

		//Sleeps one second to avoid errors
		sleep(1);
	}
}

/* MENU PLAN

"" = user input
GOTO: CAPS = go to the CAPS menu
[var] = variable

STARTUP Menu-----------------------------------------
								|					|
								|					|
								v 					v
							New Game 			Load Game
							    |					|
							    |					|
							    v 					v
							Village Name 		Choose File
							    |					|
							    |					|
							    v 					v
							GOTO: GAME 			GOTO: GAME


GAME Menu--------------------------------------------------------------------------------------------------------------------------------------------
								|				|					|					|					|					|					|
								|				|					|					|					|					|					|
								v 				v 					v 					v 					v 					v 					v
							  "Map"			"Villagers"			"Buildings"			"Resources"			 "Save"				"History"			"Advance"
							    |				|					|					|					|					|					|
							    |				|					|					|					|					|					|
							    v 				v 					v 					v 					v 					v 					v
							Prints Map 		GOTO: VILLAGER 		GOTO: BUILDINGS 	Prints Res Info 	Save Game 			Print Lst 10 Evnts	Advance a Day


VILLAGER Menu--------------------------------------------------------------------------------
								|					|					|					|
								|					|					|					|
								v 					v 					v 					v
							  "List"			"Info [name]" 		"Set [job] [name]"	"Return"
							    |					|					|					|
							    |					|					|					|
							    v 					v 					v 					v
							Lists Villagers 	Prints Vil Info 	Set Villager Job 	GOTO: GAME


BUILDINGS Menu---------------------------------------------------------------------------------------------------------------------------------------------------
								|					|					|					|					|						|						|
								|					|					|					|					|						|						|
								v 					v 					v 					v 					v 						v 						v
							  "List"			"Info [x, y]" 		"Upgrade [x, y]"	  "Map"				"Build [type] [x, y]"	"Repair [x, y]"			"Return"
							    |					|					|					|					|						|						|
							    |					|					|					|					|						|						|
							    v 					v 					v 					v 					v 						v 						v
							Lists Bldgs 		Prints Bldg Info 	Confirm Price 		Prints Map 			Confirm Price 			Confirm Price 			GOTO: GAME
																		| 										| 						|
																		| 										|						|
																		v 										v 						v
																	Upgrades Bldg  							Build Bldg 				Repair Bldg
*/
