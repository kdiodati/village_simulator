#include <iostream>
#include <exception>
#include <vector>
#include <utility>
#include <unordered_map>
#include <fstream>
#include <algorithm>

#include "json.hpp"

#include "tile.hpp"
#include "map.hpp"
#include "villagers.hpp"

// Use this to find an error by line and file
#define CHECK_ERROR std::cout << "CHECK_ERROR on line " << __LINE__ << " in " << __FILE__ << "." << std::endl;

#define LINE_LENGTH 32


std::unordered_map<std::string, std::pair<std::vector<std::string>, std::vector<std::pair<std::string, int>>>> Map::readRequirements() {
    // Read json file for parsing
    using json = nlohmann::json;
    std::ifstream i("building_requirements.json");
    json j = json::parse(i);

    // Map will hold raw data initially before being translated into the objects
    std::unordered_map<std::string, std::pair<std::vector<std::string>, std::vector<std::pair<std::string, int>>>> rawMap;    
    
    // Iterate over JSON data
    for(auto& building : j.items()) {
        std::string rawBuilding = building.key();
        std::vector<std::string> rawTiles = j[building.key()]["Tiles"].get<std::vector<std::string>>();
        std::vector<std::pair<std::string, int>> rawResources;
        for(auto& resource : j[building.key()]["Resources"].items()) {
            std::string rawResource = resource.key();
            int rawAmount = resource.value();
            std::pair<std::string, int> rawResourcePair(rawResource, rawAmount);
            rawResources.push_back(rawResourcePair);
        }
        std::pair<std::vector<std::string>, std::vector<std::pair<std::string, int>>> rawInfo(rawTiles, rawResources);
        
        // Add each building's requirements to the map
        rawMap[rawBuilding] = rawInfo;
    }

    return rawMap;
}

std::unordered_map<BuildingType, std::pair<std::vector<TileType>, std::vector<std::pair<ResourceType, int>>>, BuildingTypeHash> Map::getRequirements() {
    std::unordered_map<BuildingType, std::pair<std::vector<TileType>, std::vector<std::pair<ResourceType, int>>>, BuildingTypeHash> tempMap;
    
    std::unordered_map<std::string, std::pair<std::vector<std::string>, std::vector<std::pair<std::string, int>>>> rawMap = readRequirements();

    for(auto& pair : rawMap) {
        // BuildingType is the key
        BuildingType building = Building::parseBuildingType(pair.first);
        
        // Translate tiles
        std::vector<TileType> tiles;
        auto TILES = pair.second.first;
        for(auto t : TILES) {
            TileType tile = parseTileType(t);
            tiles.push_back(tile);
        }

        // Translate resources
        std::vector<std::pair<ResourceType, int>> resources;
        auto RESOURCES = pair.second.second;
        for(auto r : RESOURCES) {
            ResourceType resource = parseResourceType(r.first);
            int amount = r.second;
            std::pair<ResourceType, int> bundle(resource, amount);
            resources.push_back(bundle);
        }
        std::pair<std::vector<TileType>, std::vector<std::pair<ResourceType, int>>> requirements(tiles, resources);
        
        // Add to BUILDING_REQUIREMENTS
        tempMap[building] = requirements;
    }
    return tempMap;
}

Map::Map() {
    this->name = "Null";
    
    // Map Tiles
    this->tiles.push_back(Tile(TileType::Plains));
    this->tiles.push_back(Tile(TileType::Plains));
    this->tiles.push_back(Tile(TileType::Plains));
    this->tiles.push_back(Tile(TileType::Lake));
    this->tiles.push_back(Tile(TileType::Forest));
    this->tiles.push_back(Tile(TileType::Forest));
    this->tiles.push_back(Tile(TileType::Forest));
    this->tiles.push_back(Tile(TileType::Mountains));
    this->tiles.push_back(Tile(TileType::Mountains));

    // Starting Resources
    try {
        this->logs = new Resource(ResourceType::Logs, 100);
        this->stone = new Resource(ResourceType::Stone, 100);
        this->food = new Resource(ResourceType::Food, 100);
        this->water = new Resource(ResourceType::Water, 100);
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    // Village Startup
    for(unsigned int i = 0; i < 5; ++i) {
        this->village.addCitizen();
    }
}

Map::~Map() {
    delete this->logs;
    delete this->stone;
    delete this->food;
    delete this->water;
}

void Map::view() {
    // Map: 62x32 Interior: 60x30 Tiles: 20x10
    
    // Header
    std::cout << "+" << std::string(TILE_SIZE * 3 * 2, '-') << "+" << std::endl;

    for(unsigned int tileIndex = 0; tileIndex < 9; tileIndex++) {
        std::vector<std::string> leftTile = this->tiles[tileIndex].display();
        std::vector<std::string> centerTile = this->tiles[++tileIndex].display();
        std::vector<std::string> rightTile = this->tiles[++tileIndex].display();
        for(unsigned int lineIndex = 0; lineIndex < TILE_SIZE; lineIndex++) {
            std::cout << "|" << leftTile[lineIndex] << centerTile[lineIndex] << rightTile[lineIndex] << "|" << std::endl;
        } 
    }

    // Footer
    std::cout << "+" << std::string(TILE_SIZE * 3 * 2, '-') << "+" << std::endl;
}

void Map::detailedView() {
    std::cout << "+" << std::string(LINE_LENGTH * 3, '-') << "+" << std::endl;
    for(unsigned int i = 0; i < 9; i += 3) {
        
        std::string tile1 = parseTileString(this->tiles[i].getType());
        std::string tile2 = parseTileString(this->tiles[i+1].getType());
        std::string tile3 = parseTileString(this->tiles[i+2].getType());
        
        int length1 = LINE_LENGTH - tile1.length();
        int length2 = LINE_LENGTH - tile2.length();
        int length3 = LINE_LENGTH - tile3.length();
        
        // Print out tile names
        std::cout << "ID:" << i << " " << tile1 << std::string(length1, ' ');        
        std::cout << "ID:" << i+1 << " " << tile2 << std::string(length2, ' ');        
        std::cout << "ID:" << i+2 << " " << tile3 << std::string(length3, ' ') << std::endl;
        
        // Building iterators
        auto it1 = this->buildings[this->tiles[i]].begin();
        auto it2 = this->buildings[this->tiles[i+1]].begin();
        auto it3 = this->buildings[this->tiles[i+2]].begin();
        
        while(true) {
            bool exit1 = false;
            bool exit2 = false;
            bool exit3 = false;

            if(it1 != this->buildings[this->tiles[i]].end()) {
                std::string building = Building::parseBuildingString(it1->getType());
                int length = LINE_LENGTH - building.length();
                std::cout << building << ": LVL " << it1->getLevel() << " [" << this->jobs[(*it1)].size() << "/4] +" << it1->getOutput().getCount() * this->jobs[*it1].size() << std::string(length - 16, ' ');            
                ++it1;
            } else {
                exit1 = true;
                std::cout << std::string(LINE_LENGTH, ' ');
            }
            
            if(it2 != this->buildings[this->tiles[i+1]].end()) {
                std::string building = Building::parseBuildingString(it2->getType());
                int length = LINE_LENGTH - building.length();
                std::cout << building << ": LVL " << it2->getLevel() << " [" << this->jobs[(*it2)].size() << "/4] +" << it2->getOutput().getCount() * this->jobs[*it2].size() << std::string(length - 16, ' ');            
                ++it2;
            } else {
                exit2 = true;
                std::cout << std::string(LINE_LENGTH, ' ');
            }
            
            if(it3 != this->buildings[this->tiles[i+2]].end()) {
                std::string building = Building::parseBuildingString(it3->getType());
                int length = LINE_LENGTH - building.length();
                std::cout << building << ": LVL " << it3->getLevel() << " [" << this->jobs[(*it3)].size() << "/4] +" << it3->getOutput().getCount() * this->jobs[*it3].size() << std::string(length - 16, ' ');            
                ++it3;
            } else {
                exit3 = true;
                std::cout << std::string(LINE_LENGTH, ' ') << std::endl;
            }
            std::cout << std::endl;    
            if(exit1 && exit2 && exit3) {
                std::cout << "+" << std::string(LINE_LENGTH * 3, '-') << "+" << std::endl;
                break;
            }
        }
    }
}

bool Map::exists(Tile t, Building b) {
    // Returns true if the building already exists or if one of the same type

    // Make sure a building of the same type is not on the tile
    auto tileBuildings = this->buildings[t];
    for(auto it = tileBuildings.begin(); it != tileBuildings.end(); ++it) {
        // If a building of the same type already exists on the tile don't build
        if(it->getType() == b.getType()) {
            return true;
        }
    }
    return false;
}

bool Map::build(Tile t, Building b) {
    // If it exists don't build

    if(this->exists(t, b)) {
        std::cout << "Building already exists!" << std::endl;
        std::cout << Building::parseBuildingString(b.getType()) << std::endl;
        return false;
    }
    // Check the build requirements

    // (1) Check that the building can be built on the tile type
    auto okayTiles = this->BUILDING_REQUIREMENTS[b.getType()].first;
    if(std::find(okayTiles.begin(), okayTiles.end(), t.getType()) != okayTiles.end()) {
        // (2) Check that the resources are available
        auto resourceCosts = this->BUILDING_REQUIREMENTS[b.getType()].second;
        for(auto it = resourceCosts.begin(); it != resourceCosts.end(); ++it) {
            if(it->first == ResourceType::Logs) {
                if(it->second > this->logs->getCount()) {
                    std::cout << it->second << ", " << this->logs->getCount() << std::endl;
                    std::cout << "Not enough logs!" << std::endl;
                    return false;
                } 
            } else if(it->first == ResourceType::Water) {
                if(it->second > this->water->getCount()) {
                    std::cout << "Not enough water!" << std::endl;
                    return false;
                } 
            } else if(it->first == ResourceType::Food) {
                if(it->second > this->food->getCount()) {
                    std::cout << "Not enough food!" << std::endl;
                    return false;
                } 
            } else if(it->first == ResourceType::Stone) {
                if(it->second > this->stone->getCount()) {
                    std::cout << "Not enough stone!" << std::endl;
                    return false;
                } 
            }
        }

        // Resources are available so consume them
        
        for(auto it = resourceCosts.begin(); it != resourceCosts.end(); ++it) {
            if(it->first == ResourceType::Logs) {
                this->logs->changeCount(-1 * it->second);
            } else if(it->first == ResourceType::Water) {
                this->water->changeCount(-1 * it->second);
            } else if(it->first == ResourceType::Food) {
                this->food->changeCount(-1 * it->second);
            } else if(it->first == ResourceType::Stone) {
                this->stone->changeCount(-1 * it->second);
            }
        }
        
        this->buildings[t].push_back(b);
        std::vector<Villager> villagers;
        //this->jobs[b] = villagers;
        return true;
    }
    // Specified tile not in building requirements don't build
    std::cout << "Cannot be built on this tile!" << std::endl;
    return false;
}

bool Map::demolish(Tile t, Building b) {
    if(this->exists(t, b)) {
        auto tileBuildings = this->buildings[t];
        for(auto it = tileBuildings.begin(); it != tileBuildings.end(); ++it) {
            // If the building in the tile is the same as the one passed
            if((*it) == b) {
                std::cout << this->jobs[*it].size() << std::endl;
                if (this->jobs[*it].size() == 0) {
                    this->buildings[t].clear();
                    return true;
                }
                // Unassign villagers
                for (auto v = this->jobs[*it].rbegin(); v != this->jobs[*it].rend(); ++v) {
                    this->unassign(v->getId());
                }
                // Delete and update tile's buildings
                this->unassign(this->jobs[*it][0].getId() - 1);
                tileBuildings.erase(it);
                this->buildings[t] = tileBuildings;
                return true;
            }
        }
    }
    // Building not found don't demolish
    return false;
}

bool Map::upgrade(Tile t, Building b) {
    // 50 logs and 50 stone for upgrading
    if(this->getLogs()->getCount() < 50) {
        std::cout << "Not enough logs!" << std::endl;
        return false;
    }
    if(this->getStone()->getCount() < 50) {
        std::cout << "Not enough stone!" << std::endl;
        return false;
    }
    this->addLogs(-50);
    this->addStone(-50);
    if(this->exists(t, b)) {
        auto tileBuildings = this->buildings[t];
        for(auto it = tileBuildings.begin(); it != tileBuildings.end();) {
            // If the building in the tile is the same as the one passed
            if((*it) == b) {
                b = (*it);
                b.upgrade();
                // Unassign villagers
                if (this->jobs[*it].size() == 0) {
                    this->buildings[t].clear();
                    it = tileBuildings.erase(it);
                    tileBuildings.push_back(b);
                    this->buildings[t] = tileBuildings;
                    return true;
                }
                for (auto v = this->jobs[*it].rbegin(); v != this->jobs[*it].rend(); ++v) {
                    this->unassign(v->getId());
                }
                this->unassign(this->jobs[*it][0].getId() - 1);
                it = tileBuildings.erase(it);
                tileBuildings.push_back(b);
                this->buildings[t] = tileBuildings;
                return true;
            }
            ++it;
        }
    }
    // Cannot upgrade a building that does not exist
    return false;
}

void Map::setName(std::string name_) {
	this->name = name_;
}

Resource *Map::getLogs() {
    return this->logs;
}

Resource *Map::getStone() {
    return this->stone;
}

Resource *Map::getWater() {
    return this->water;
}

Resource *Map::getFood() {
    return this->food;
}

std::string Map::getName() {
    return this->name;
}

std::vector<Tile> Map::getTiles() {
    return this->tiles;
}

std::unordered_map<Tile, std::vector<Building>> Map::getBuildings() {
    return this->buildings;
}

//////////////////////////////////////////
// --VILLAGE FUNCTIONS--
    // Getter
Village Map::getVillage() {
    return this->village;
}

    // Setter
void Map::setVillage(Village village_) {
    this->village = village_;
}

void Map::deathCheck() {
	std::vector<int> v_ = this->village.getDead();
	if (v_.size() > 0) {
		for (unsigned int i=0; i < v_.size(); i++) {
			int index = v_[i];
			this->unassign(index);
			//std::cout << this->village.getVector()[index].getName() << " has died." << std::endl; //testing
			this->village.removeCitizen(index); 
			this->village.incrementDeaths();
		}
	}
}

//////////////////////////////////////////


void Map::addLogs(int amount) {
    this->logs->changeCount(amount);
}

void Map::addWater(int amount) {
    this->water->changeCount(amount);
}


void Map::addFood(int amount) {
    this->food->changeCount(amount);
}

void Map::addStone(int amount) {
    this->stone->changeCount(amount);
}

void Map::getResources() {
    std::cout << "Logs: " << this->logs->getCount() << std::endl;
    std::cout << "Stone: " << this->stone->getCount() << std::endl;
    std::cout << "Food: " << this->food->getCount() << std::endl;
    std::cout << "Water: " << this->water->getCount() << std::endl;
}

void Map::collect() {
    // Colllect resources from buildings
    for(auto it = this->tiles.begin(); it != this->tiles.end(); ++it) {
        for(auto b = this->buildings[(*it)].begin(); b != this->buildings[(*it)].end(); ++b) {
            std::cout << Building::parseBuildingString(b->getType()) << " LVL " << b->getLevel() << " produces ";
            Resource r = (*b).getOutput();
            if(r.getType() == ResourceType::Food) {
                std::cout << r.getCount() * this->jobs[*b].size() << " food!" << std::endl;
                this->addFood(r.getCount() * this->jobs[(*b)].size());
            } else if(r.getType() == ResourceType::Water) {
                this->addWater(r.getCount() * this->jobs[(*b)].size());
                std::cout << r.getCount() * this->jobs[*b].size() << " water!" << std::endl;
            } else if(r.getType() == ResourceType::Logs) {
                this->addLogs(r.getCount() * this->jobs[(*b)].size());
                std::cout << r.getCount()  * this->jobs[*b].size() << " logs!" << std::endl;
            } else if(r.getType() == ResourceType::Stone) {
                this->addStone(r.getCount() * this->jobs[(*b)].size());
                std::cout << r.getCount() * this->jobs[*b].size() << " stone!" << std::endl;
            } 
        }
    }
}

void Map::consume() {
    // Consume food and water for the villagers
    int food_consumed = this->food->getCount() - this->village.feed(this->food->getCount());
    std::cout << food_consumed << " food consumed." << std::endl;
    this->addFood(-1 * food_consumed);
    int water_consumed = this->water->getCount() - this->village.water(this->water->getCount());
    std::cout << water_consumed << " water consumed." << std::endl; 
    this->addWater(-1 * water_consumed);
}

std::unordered_map<Building, std::vector<Villager>> Map::getJobs() {
    return this->jobs;
}

bool Map::assign(int i, Building b) {
    int free_villager = this->village.getJobless();
    // No free villagers
    if(free_villager == -1) {
        std::cout << "No free villagers!" << std::endl;
        return false;
    }
    // No free space
    if(this->jobs[b].size() == 4) {
        std::cout << "Building is full!" << std::endl;
        return false;
    }
    
    
    Villager v = this->village.getVector()[free_villager];
    if(b.getType() == BuildingType::Farm) {
        this->village.makeFarmer(free_villager);    
    } else if(b.getType() == BuildingType::Quarry) {
        this->village.makeMiner(free_villager);    
    } else if(b.getType() == BuildingType::Lumberjack_Hut) {
        this->village.makeLumberjack(free_villager);    
    } else if(b.getType() == BuildingType::Water_Source) {
        this->village.makeFisher(free_villager);
    }
    
    // Find the building index
    Tile t = this->tiles[i];
    auto it = std::find(this->buildings[t].begin(), this->buildings[t].end(), b);
    int index = std::distance(this->buildings[t].begin(), it);
    v.setWorkTile(i);
    v.setWorkBldg(index);
    this->jobs[b].push_back(v);
    return true;
}


bool Map::unassign(int i) {
    Villager v = this->village.getVector()[i];
    if(v.getJob() != "none") {
        // Remove last job
        Tile t = this->tiles[v.getWorkTile() + 1];
        Building b = this->buildings[t][v.getWorkBldg() + 1];
        for(auto it = this->jobs[b].begin(); it != this->jobs[b].end(); ++it) {
            if(it->getName() == v.getName()) {
                this->jobs[b].erase(it);
                this->village.clearJob(i);
                return true;
            }
        }
    }
    std::cout << "This villager " << v.getName() << " is already unassigned!" << std::endl;
    return false;
}

std::unordered_map<int, BuildingType> Map::availableBuildings(int i) {
    std::cout << parseTileString(this->tiles[i].getType()) << std::endl;
    std::vector<std::string> buildings;

    // Output 
    std::unordered_map<int, BuildingType> poss;

    // Print the available building for the tile
    for(auto it = this->BUILDING_REQUIREMENTS.begin(); it != this->BUILDING_REQUIREMENTS.end(); ++it) {
        auto possB = (*it).second.first;
        for(auto itB = possB.begin(); itB != possB.end(); ++itB) {
            if((*itB) == this->tiles[i].getType()) {
                buildings.push_back(Building::parseBuildingString((*it).first));
            }
        }
    }
    i = 0;
    for(auto it = buildings.begin(); it != buildings.end(); ++it) {
        poss[i] = Building::parseBuildingType((*it));
        std::cout << i++ << ": " << (*it) << std::endl;
    }

    return poss;
}

void Map::listBuildings(int i) {
    // List buildings that are built
    std::unordered_map<int, std::string> bld;
    auto tile = this->tiles[i];
    int j = 0;
    for(auto it = this->buildings[tile].begin(); it != this->buildings[tile].end(); ++it) {
        std::cout << j << ": " << Building::parseBuildingString(it->getType()) << std::endl;
        bld[j++] = Building::parseBuildingString(it->getType());
    }
}
