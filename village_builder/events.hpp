#ifndef EVENTS_HPP_
#define EVENTS_HPP_

#include <string>
#include <vector>
#include "villagers.hpp"
#include "resource.hpp"

//Storm, Fire, Settler, Harvest, Rain, Infestation, Contamination, Animal, Raid, Winter
//  1      2    10-15      3      4         5             6           7      8      9  :Run if

	/*
	//setters

	void setVillage(Village v_);

	void setVect(std::vector<Villager> vect_);

	//getters

	Village getVillage();

	std::vector<Villager> getVect();

	*/

	//functions

	//void runEvent(); //-1 means do none, 1 means storm, 2 means...
		
	int doStorm(int r);

	int doFire(int r);

	int doSettler(std::mt19937 rng);

	int doHarvest(int r);

	int doRain(int r);

	int doInfestation(int r);

	int doContamination(int r);

	int doAnimal(std::mt19937 rng);

	int doRaid(std::mt19937 rng);

	int doWinter(int r);

	//helpers

	bool deathChance(int p, std::mt19937 rng); //will return random villager index to kill if random < p
	//use Village.getRng() for random to keep randomization true random

	int stormFire(int r); //return ~10% of total resource

	//void addSettler(int i); //adds 1-2 villagers to village

	int doubleSupply(int r); //doubles food get for that day

	int halfSupply(int r); //doubles water get for that day

#endif
