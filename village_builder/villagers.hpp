#ifndef VILLAGERS_HPP_
#define VILLAGERS_HPP_

#include <vector>
#include <string>
#include <random>

class Villager {
	private:
		int id = -1;
		bool alive = 1;
		std::string name = "NULL";
		int foodLevel = 100;
		int waterLevel = 100;
		std::string job = "NULL";
		int workTile = -1;
		int workBldg = -1;
		std::uniform_int_distribution<int> starve; //10-25
	public:
		//    Constructors

		//Villager(std::string name_) : name(name_) {}

		//Villager(std::string name_, int id_) : name(name_), id(id_) {}
		
		Villager();

		//    Getters
		
		int getWorkTile();

		int getWorkBldg();

		int getId();

		std::string getName();

		int getFoodLevel();

		bool isAlive();

		int getWaterLevel();

		std::string getJob();

		//   Setters
		
		void setWorkTile(int i);

		void setWorkBldg(int i);

		void makeDead();

		void setId(int id_);

		void setName(std::string name_);

		void setJob(std::string job_);

		void hunger();

		//void hunger(std::mt19937 rng);

		void thirst();
		
		//void thirst(std::mt19937 rng);

		void full();

		void drink();

		//   Printers

		void printInfo();
};

class Village {
	private:
		int count = 0;
		int deaths = 0;

		std::mt19937 rng;
		std::uniform_int_distribution<int> dist; //0-19

		std::vector<Villager> citizens;

		//temporary arrays of possible names (first and last)

		std::vector<std::string> firstNames;

		std::vector<std::string> lastNames;

		/*std::string firstName[20] = {"Kyle", "Carter", "Taylor", "James", "John", "Robert",
		       	"Michael", "William", "David", "Richard", "Mary", "Patricia", "Linda",
		       	"Barbara", "Elizabeth", "Jennifer", "Maria", "Susan", "Margaret", "Dorothy"};

		std::string lastName[20] = {"Diodati", "Perkins", "Scott", "Smith", "Johnson",
		       	"Williams", "Jones", "Brown", "Davis", "Miller", "Wilson", "Moore", "Taylor",
			"Anderson", "Thomas", "Jackson", "White", "Harris", "Martin", "Thompson"};*/
	public:
		//Constructor
		
		Village();

		//Getters
		
		std::mt19937 getRng();

		int getCount();

		int getDeaths();

		std::vector<Villager> getVector();

		int searchVillage(int id_);

		int checkName(std::string name_);
		
		int getJobless();

		std::vector<int> getDead();

		//Setters

		bool clearJob(int i);

		void makeFarmer(int i);

		bool makeFisher(int i);

		bool makeLumberjack(int i);

		bool makeMiner(int i);

		bool makeBuilder(int i);

		bool addCitizen();

		bool removeCitizen(int id_);

		void incrementCount();

		void incrementDeaths();

		bool setCount(int count_);

		std::string makeName();

		void printVillagerInfo(int i);

		void printJobs();

		void printVillage();

		void printHunger();

		void killRand();

		int feed(int r);

		int water(int r);

		void starving();

		void feast();

		void quench();
};

#endif
