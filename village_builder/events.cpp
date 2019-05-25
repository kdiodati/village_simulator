#include "events.hpp"

#include <string>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include "villagers.hpp"
#include "resource.hpp"

using namespace std;

// Use this to find an error by line and file
#define CHECK_ERROR cout << "CHECK_ERROR on line " << __LINE__ << " in " << __FILE__ << "." << endl;


//functions

string makeLower(string s) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}


int doStorm(int r) {
	int i = stormFire(r);
	cout << endl << "A storm hits your village, use " << i << " stone for repairs." << endl; 
	return i;
}

int doFire(int r) {
	int i = stormFire(r);
	cout << endl << "A fire occurs in your village, use " << i << " wood for repairs." << endl;
	return i;
}

int doSettler(mt19937 rng) {
	uniform_int_distribution<int> d(1,2);
	int i = d(rng);
	while(true) {
		string buffer;
		if (i == 1) {
			cout << endl << "A Villager shows up to your village, do you want to let them in?" << endl;
		}
		else {
			cout << endl << "Two Villagers show up to your village, do you want to let them in?" << endl;
		}
		cout << "Y/N >";
		cin >> buffer;
		buffer = makeLower(buffer);
		if (buffer == "no" || buffer == "n") {
			return -1;
		}
		else if (buffer == "yes" || buffer == "y") {
			return i;
		}
		cout << "Error: Command not recognized! Enter 'yes' or 'no'" << endl;
	}
}

int doHarvest(int r) {
	cout << endl << "There was a plentiful Harvest, +10 food." << endl;
	return 10;
}

int doRain(int r) {
	cout << endl << "It rained last night, +10 water." << endl;
	return 10;
}

int doInfestation(int r) {
	int i = halfSupply(r);
	cout << endl << "There was an infestation in our food supply, we lost half our stored food, -" << i << " food." << endl;
	return i;
}

int doContamination(int r) {
	int i = halfSupply(r);
	cout << endl << "Our supply of fresh water got contaminated, we lost half our stored water, -" << i << " water." << endl;
	return i;
}

int doAnimal(mt19937 rng) {
	cout << endl << "There was a wild animal attack... ";
	if (deathChance(20, rng)) {
		cout << "It killed one of your villagers before running off." << endl;
		return -1;
	}
	uniform_int_distribution<int> d(5,25);
	int i = d(rng);
	cout << "Your villagers managed to kill it, +" << i << " food." << endl; 
	return i; //food
}

int doRaid(mt19937 rng) {
	cout << endl << "There was a raid by some bandits... ";
	uniform_int_distribution<int> d(5,20);
	int i = d(rng);
	if (deathChance(25, rng)) {
		cout << "They killed one of your villagers, and stole " << i << " food." << endl;
		i = i * -1;
		return i; //will be negative if villager killed
	}
	cout << "They stole " << i << " food." << endl;
	return i; //food to subtract (positive if village alive).
}

int doWinter(int r) {
	int i = r / 5; // - 20%
	cout << endl << "A snowstorm hits your village, you lose " << i << " food." << endl;
	return i;
}

//helpers

bool deathChance(int p, mt19937 rng) {
	uniform_int_distribution<int> d(1,100);
	int i = d(rng);
	if (i <= p) {
		return true;
	}
	return false;
}

int stormFire(int r) {
	if (r == 0) {
		return 0;
	}
	else if (r < 10) {
		return (1);
	}
	return (r / 10);
}

int doubleSupply(int r) {
	return r;
}

int halfSupply(int r) {
	return (r / 2);
}
