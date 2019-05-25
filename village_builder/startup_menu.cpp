#include "game_menu.hpp"
//#include "map.hpp"
#include "startup_menu.hpp"
#include <iostream>
#include <string>
#include <algorithm>

// Use this to find an error by line and file
#define CHECK_ERROR cout << "CHECK_ERROR on line " << __LINE__ << " in " << __FILE__ << "." << endl;

#define BORDER "================================================================================"
#define SPLITTER cout << "--------------------------------------------------------------------------------" << endl;
#define VERSION "0.3"

using namespace std;

string CheckInput(string in) {
	// Chnages the input string to be lowercase for easier parsing.
	transform(in.begin(), in.end(), in.begin(), ::tolower);
	if (in == "help") {
		cout << BORDER << endl;

		cout << "                          COMMANDS                          " << endl;
		// Commands that will always appear at the top.
		cout << "'Help' - Opens list of commands." << endl;
		cout << "'Quit' - Exits the program." << endl;
		cout << "Follow the prompts to start the game." << endl;

		cout << BORDER << endl;
	}
	else if(in == "quit") {
		cout << "Quitting Game..." << endl;
		exit(0);
	}
	return in;
}

string RunStartupMenu() {
	Welcome();
	string buffer;
	while(true) {
		cout << "Please enter 'New' to start a new game or 'Load' to load a save file." << endl;
		cout << "N/L >";
		cin >> buffer;
		SPLITTER
		buffer = CheckInput(buffer);
		// Create a new game
		if (buffer == "new" || buffer == "n") {
			//Map map("Null");
			//map.setName(NewGame());
			//Map map = NewGame();
			return NewGame();
		}
		else if (buffer == "load" || buffer == "l") {
			cout << "Load functionality is not available at this time." << endl;
		}
		else if (buffer == "return") {
			continue;
		}
		else {
			cout << "Invalid command: enter 'Help' for a list of commands." << endl;
		}
	}
}

// Prints welcome message
void Welcome() {
	string buffer;
	cout << "Welcome to Village Builder! (" << VERSION << ")" << endl;
	cout << "=== At any time during gameplay, enter 'Help' for a list of commands ===" << endl;
	cout << "=== For the cleanest experience, set your terminal window to at least 80 characters wide ===" << endl;
	cout << endl;
	while(true) {
		cout << "Would you like to know more about the game before you begin?" << endl;
		cout << "Y/N >";
		cin >> buffer;
		buffer = CheckInput(buffer);
		if (buffer == "yes" || buffer == "y") {
			cout << "In Village Builder, you are in charge of managing a small village and helping it grow." << endl;
			cout << "You begin with three villagers and it is your job to assign them jobs, construct buildings," << endl;
			cout << "and essentially manage their lives. Like any village, you must beware! Resources are tight," << endl;
			cout << "and perilous events are bound to try your people. If all of your villagers meet their end," << endl;
			cout << "be it by hunger, thirst, or a sudden plague, your game will be over. So, plan wisely, and" << endl;
			cout << "grow your village to a level of prosperity never seen before!" << endl;
			SPLITTER
			break;
		}
		else if (buffer == "no" || buffer == "n") {
			break;
		}
		else {
			cout << "Invalid Command" << endl;
			continue;
		}
	}
}

string NewGame() {
	string buffer;
	string name_;
	string name_caps;
	while(true) {
		cout << "Please enter a name for your village." << endl;
		cout << "Name >";
		cin >> buffer;
		getline(cin,name_);
		name_caps = buffer + name_;
		name_ = CheckInput(buffer + name_);
		// Makes sure name isn't too long.
		if (name_.length() > 16) {
			cout << "Your name is too long. It needs to be at most 16 characters." << endl;
		}
		else if (name_.length() < 1) {
			cout << "Your villagers need to know where they live!" << endl;
		}
		else {
			//returns the name that will be put into map
			cout << "Congratulations! You're now in charge of " << name_caps << "!" << endl;
			return name_;
		}
	}
}

//void LoadGame(ifstream &saveFile) {
	// Not implemented yet (Low Priority)
//}

/*
TEST CODE
int main() {
	RunStartupMenu();
	return 0;
}
*/



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
