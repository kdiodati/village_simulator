#ifndef MAP_HPP_
#define MAP_HPP_

#include <utility>
#include <string>
#include <vector>
#include <unordered_map>

#include "tile.hpp"
#include "building.hpp"
#include "resource.hpp"
#include "villagers.hpp"

class Map {
    
    public:
        // Constructor
        Map();
	//Map(std::string name_);
        ~Map();
        
        // Building Requirement Functions

        // Read the building requirements from "building_requirements.json"
        std::unordered_map<std::string, std::pair<std::vector<std::string>, std::vector<std::pair<std::string, int>>>> readRequirements();

        // Parse the raw data from readRequirements() into BUILDING_REQUIREMENTS
        std::unordered_map<BuildingType, std::pair<std::vector<TileType>, std::vector<std::pair<ResourceType, int>>>, BuildingTypeHash> getRequirements();

        // Building Requirements
        // TODO: Change Pair of ResourceType and int to Resource objects
        std::unordered_map<BuildingType, std::pair<std::vector<TileType>, std::vector<std::pair<ResourceType, int>>>, BuildingTypeHash> BUILDING_REQUIREMENTS = getRequirements();
        
        // Display the Map Tiles
        void view();
        // Display the Map with Details
        void detailedView();
       
        // Buildings
        bool build(Tile t, Building b); 
        bool demolish(Tile t, Building b);
        bool exists(Tile t, Building b); // does the building exist on the tile
        bool upgrade(Tile t, Building b);
        
        // Getters/Setters
        std::vector<Tile> getTiles();
        std::unordered_map<Tile, std::vector<Building>> getBuildings();
        std::string getName();
	    void setName(std::string name_);
        
        Resource *getLogs();
        Resource *getStone();
        Resource *getFood();
        Resource *getWater();

        void addLogs(int amount);
        void addStone(int amount);
        void addFood(int amount);
        void addWater(int amount);

        // --VILLAGE FUNCTIONS--

        //	getter
        Village getVillage();
        
        //	setter
        void setVillage(Village village_);
        
        // Job methods
        bool assign(int i, Building b);
        bool unassign(int i);
        
        // Advance turn calculate resources earning
        void collect();

        // Advance turn calculate resources consumed by villagers
        void consume();
        
        // Display resources
        void getResources();

	// Check if dead
	void deathCheck();

        // Get Jobs
        std::unordered_map<Building, std::vector<Villager>> getJobs();
        
        // Printing methods
       
        // Prints and returns a map of what buildings can be built on a tile
        std::unordered_map<int, BuildingType> availableBuildings(int i);
        
        // Prints and returns a map of what buildings are built on a tile
        void listBuildings(int i);

    private:
        // Name
        std::string name;

        // Player's resources
        Resource *logs;
        Resource *food;
        Resource *water;
        Resource *stone;
        
        // Player's village
        Village village;

        // Map size is 3x3 - inorder from top left corner to bottom right corner
        std::vector<Tile> tiles;

        // Unordered Map where the til is the key and a vector of buildings that are on that tile 
        std::unordered_map<Tile, std::vector<Building>> buildings;

        // Unordered Map where the building is the key and a vector of villagers for jobs that are assigned
        std::unordered_map<Building, std::vector<Villager>> jobs;
};

#endif
