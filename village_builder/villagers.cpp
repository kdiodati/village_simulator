#include "villagers.hpp"

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>
#include <fstream>

// Use this to find an error by line and file
#define CHECK_ERROR cout << "CHECK_ERROR on line " << __LINE__ << " in " << __FILE__ << "." << endl;
#define SPLITTER cout << "--------------------------------------------------------------------------------" << endl;

using namespace std;

//VILLAGERS

Villager::Villager() {
	this->id = -1;
	this->name = "NULL";
	uniform_int_distribution<int> s(10-25);
	this->starve = s;
}

//getters

int Villager::getWorkTile() {
	return this->workTile;
}

int Villager::getWorkBldg() {
	return this->workBldg;
}

int Villager::getId() {
	return this->id;
}

std::string Villager::getName() {
	return this->name;
}

bool Villager::isAlive() {
	return this->alive;
}

int Villager::getFoodLevel() {
	return this->foodLevel;
}

int Villager::getWaterLevel() {
	return this->waterLevel;
}

std::string Villager::getJob() {
	return this->job;
}

//setters

void Villager::setWorkTile(int i) {
	this->workTile = i;
}

void Villager::setWorkBldg(int i) {
	this->workBldg = i;
}

void Villager::makeDead() {
	this->alive = 0;
}

void Villager::setId(int id_) {
	this->id = id_;
}

void Villager::setName(string name_) { //create logic
	this->name = name_;
}

void Villager::setJob(string job_) {
	this->job = job_;
}

void Villager::hunger() { //TODO setup random starvation
	this->foodLevel -= 15;
	if (this->foodLevel <= 0) {
		makeDead();
	}
	//this->foodLevel = this->foodLevel - (this->starve(rng));
}

void Villager::thirst() {
	this->waterLevel -= 25;
	if (this->waterLevel <= 0) {
		makeDead();
	}
}

void Villager::full() {
	this->foodLevel = 100;
}

void Villager::drink() {
	this->waterLevel = 100;
}

//printers

void Villager::printInfo() { //'Info [name]'
	cout << "Name: " << getName() << endl;
	cout << "Job: " << getJob() << endl;
	cout << "Hunger Level: " << getFoodLevel() << endl << endl;
}

//create list of first names to randomly select from
//create list of last names to randomly select from

//create function to randomly lower hunger (if no food resources will lower anywhere from 10-25% per day






//VILLAGE

Village::Village() {
	this->count = 0;
	mt19937 r;
	this->rng = r;
	this->rng.seed(std::random_device()());
	uniform_int_distribution<int> d(0,1999);
	this->dist = d;
	string line;
  	ifstream fnfile ("info_files/first_names.txt");
  	if (fnfile.is_open())
  	{
    	while ( getline (fnfile,line) )
    	{
    		transform(line.begin()+1, line.end(), line.begin()+1, ::tolower);
      		this->firstNames.push_back(line);
    	}
    fnfile.close();
  	}
  	else cout << "Unable to open fnfile";
  	ifstream snfile ("info_files/surnames.txt");
  	if (snfile.is_open())
  	{
    	while ( getline (snfile,line) )
    	{
    		transform(line.begin()+1, line.end(), line.begin()+1, ::tolower);
      		this->lastNames.push_back(line);
    	}
    snfile.close();
  	}
  	else cout << "Unable to open snfile"; 
}

//getters

mt19937 Village::getRng() {
	return this->rng;
}

int Village::getCount() {
	return this->count;
}

int Village::getDeaths() {
	return this->deaths;
}

std::vector<Villager> Village::getVector() {
	for (int i=0; i < this->citizens.size(); i++) {
		this->citizens[i].setId(i+1);
	}
	return this->citizens;
}

int Village::searchVillage(int id_) {
	for (int i=0; i < this->citizens.size(); i++) {
		if (id_ == this->citizens[i].getId()) {
			return i; 
		}
	}
	return -1; //returns -1 if not in village, -1 can never be an id as they start at 0
}

int Village::checkName(string name_) {
	string temp;
	for (int i=0; i < this->citizens.size(); i++) {
		temp = this->citizens[i].getName();
		transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
		transform(name_.begin(), name_.end(), name_.begin(), ::tolower);
		if (name_ == temp) {
			return i;
		}
	}
	return -1;
}

int Village::getJobless() {
	for (int i=0; i < this->citizens.size(); i++) {
		if (this->citizens[i].getJob() == "none") {
			return i;
		}
	}
	return -1;
}

vector<int> Village::getDead() {
	vector<int> r;
	for (int i=this->citizens.size()-1; i >= 0; i--) {
		if (this->citizens[i].isAlive() == false) {
			//cout << i;
			r.push_back(i);
		}
		//cout << endl;
	}
	return r;
}

//setters


bool Village::clearJob(int i) {
	if (i == -1) {
		return false;
	}
	this->citizens[i].setJob("none");
	return true;
}

void Village::makeFarmer(int i) {
	this->citizens[i].setJob("farmer");
	cout << this->citizens[i].getName() << " is now a farmer." << endl;
}

bool Village::makeFisher(int i) {
	if (i == -1) {
		return false;
	}
	this->citizens[i].setJob("fisher");
	cout << this->citizens[i].getName() << " is now a fisher." << endl;
	return true;
}

bool Village::makeLumberjack(int i) {
	if (i == -1) {
		return false;
	}
	this->citizens[i].setJob("lumberjack");
	cout << this->citizens[i].getName() << " is now a lumberjack." << endl;
	return true;
}

bool Village::makeMiner(int i) {
	if (i == -1) {
		return false;
	}
	this->citizens[i].setJob("miner");
	cout << this->citizens[i].getName() << " is now a miner." << endl;
	return true;
}

bool Village::makeBuilder(int i) {
	if (i == -1) {
		return false;
	}
	this->citizens[i].setJob("builder");
	cout << this->citizens[i].getName() << " is now a builder." << endl;
	return true;
}

void Village::incrementCount() {
	this->count++;
}

void Village::incrementDeaths() {
	this->deaths++;
}

std::string Village::makeName() {
	string f = this->firstNames[this->dist(this->rng)];
	string l = this->lastNames[this->dist(this->rng)];
	return (f + " " + l);
}


bool Village::addCitizen() {
	Villager newVillager;

	//TODO Add Bool Check
	
	this->citizens.push_back(newVillager);

	//Set ID
	this->citizens.back().setId( getCount() );
	incrementCount();

	//Set Name
	this->citizens.back().setName( makeName() );

	//Set Jobless
	this->citizens.back().setJob("none");

	return true;
}

bool Village::removeCitizen(int i) {

	if (i == -1) {
		return false;
	}
	this->citizens.erase(this->citizens.begin()+i);
	return true;

	/*
	//temp patch code until can get iterator fully functioning
	for (int i=0; i < this->citizens.size(); i++) {
		if (this->citizens[i].getId() == id_) {
			this->citizens.erase(this->citizens.begin()+i);
			incrementDeaths();
			return true;
		}
	}
	return false;
	*/
}

bool Village::setCount(int count_) {
	this->count = count_;
	return true;
}

void Village::printVillagerInfo(int i) {
	if (i != -1) {
		this->citizens[i].printInfo();
	}
}

void Village::printJobs() {
	int farmer = 0;
	int fisher = 0;
	int lumberjack = 0;
	int miner = 0;
	int builder = 0;
	int jobless = 0;

	string job;

	for (int i=0; i < this->citizens.size(); i++) {
		job = this->citizens[i].getJob();
		if (job == "farmer") {
			farmer++;
		}
		else if (job == "fisher") {
			fisher++;
		}
		else if (job == "lumberjack") {
			lumberjack++;
		}
		else if (job == "miner") {
			miner++;
		}
		else if (job == "builder") {
			builder++;
		}
		else {
			jobless++;
		}
	}

	cout << "Total Village Population: " << this->citizens.size() << endl;
	cout << "Farmers: " << farmer << endl;
	cout << "Fishers: " << fisher << endl;
	cout << "Lumberjacks: " << lumberjack << endl;
	cout << "Miners: " << miner << endl;
	cout << "Builders: " << builder << endl;
	cout << "Jobless: " << jobless << endl;
}

void Village::printVillage() {
	string job_upper;
	cout << "ID  Name                      Job             Hunger          Thirst" << endl;
	SPLITTER
	for (int i=0; i<this->citizens.size(); i++) {
		job_upper = this->citizens[i].getJob();
		job_upper[0] = toupper(job_upper[0]);
		cout << setw(4) << left << i;
		cout << setw(26) << left << this->citizens[i].getName();
		cout << setw(16) << job_upper;
		cout << setw(16) << left << this->citizens[i].getFoodLevel();
		cout << setw(4) << left << this->citizens[i].getWaterLevel() << endl;
		/*if (i != citizens.size() - 1) {
			cout << ", ";
		}
		if ((i+1) % 6 == 0) {
			cout << endl;
		}*/
	}
}

void Village::printHunger() {
	for (int i=0; i<this->citizens.size(); i++) {
		cout << this->citizens[i].getName() << ": " << this->citizens[i].getFoodLevel();
		if (i != citizens.size() - 1) {
			cout << ", ";
		}
		if ((i+1) % 4 == 0) {
			cout << endl;
		}
	}
	cout << endl;
}

void Village::killRand() {
	uniform_int_distribution<int> l(0, this->citizens.size()-1);
	int i = l(rng);
	this->citizens[i].makeDead();
}

int Village::feed(int r) {
	for (int i=0; i<this->citizens.size(); i++) {
		if (r != 0) {
			this->citizens[i].full();
			r--;
		}
		else {
			this->citizens[i].hunger();
		}
	}
	return r;
}

int Village::water(int r) {
	for (int i=0; i<this->citizens.size(); i++) {
		if (r != 0) {
			this->citizens[i].drink();
			r--;
		}
		else {
			this->citizens[i].thirst();
		}
	}
	return r;
}

void Village::quench() {
	for (int i=0; i < citizens.size(); i++) {
		this->citizens[i].drink();
	}
}

void Village::feast() {
	for (int i=0; i < citizens.size(); i++) {
		this->citizens[i].full();
	}
}


void Village::starving() {//FIXME
	for (int i=0; i < citizens.size(); i++) {
		this->citizens[i].hunger();
	}
}

/*

//Main used for testing purposes
int main( int argc, const char* argv[] ) {

	Village village;
	vector<Villager> v;

	village.addCitizen(); //0
	village.printVillage();

	village.addCitizen(); //1
	village.printVillage();
	village.makeFarmer(0);
	village.makeLumberjack(1);
	

	v = village.getVector();


//	cout << village.makeFarmer(0) << " == 1?\n"; //test to see if it returns true
	cout << v[0].getName() << " is a " << v[0].getJob() << "   (should be farmer)" << endl;
	cout << v[1].getName() << " is a " << v[1].getJob() << "   (should be lumberjack)" << endl;

	v[0].printInfo();

	village.removeCitizen(0);
	village.printVillage();
//	cout << village.makeFisher(0) << " == 0?\n"; //test to see if it returns false
	
	village.addCitizen(); //2  //1, 2 alive
	village.addCitizen(); //3  //1, 2, 3 alive
	village.removeCitizen(3);  //1, 2 alive
	village.addCitizen(); //4  //1, 2, 4 alive

	v = village.getVector();

	 

	for (int i=0; i < v.size(); i++) {
		cout << v[i].getId() << " ";
	}
	cout << endl;
		

	for (int i=0; i < 30; i++) {
		village.addCitizen();
	}


	village.printVillage();
	cout << endl;
	village.printHunger();
	cout << endl;
	village.starving();
	village.printHunger();
	
	
	village.printVillage();
	
	village.makeFarmer(village.getJobless());

	while (village.getJobless() != -1) {
		village.makeMiner(village.getJobless());
	}

	v = village.getVector();

	for (int i=0; i < v.size(); i++) {
		cout << v[i].getJob() << " ";
	}
	cout << endl;
	
}

 */
