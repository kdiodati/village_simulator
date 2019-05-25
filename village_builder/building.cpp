#include <string>
#include <iostream>
#include <unordered_map>

#include "resource.hpp"
#include "building.hpp"
#include "tile.hpp"

#define MAX_LEVEL 3
#define RESOURCE_OUTPUT 2
#define CHECK_ERROR std::cout << "CHECK_ERROR on line " << __LINE__ << " in " << __FILE__ << "." << std::endl;

int Building::uID = 0;

BuildingType Building::parseBuildingType(std::string str) {
    // Table of strings to BuildingTypes    
    std::unordered_map<std::string, BuildingType> table;
    table["Farm"] = BuildingType::Farm;
    table["House"] = BuildingType::House;
    table["Lumberjack Hut"] = BuildingType::Lumberjack_Hut;
    table["Quarry"] = BuildingType::Quarry;
    table["Water Source"] = BuildingType::Water_Source;
    return table[str];
}

std::string Building::parseBuildingString(BuildingType b) {
    // Table of BuildingTypes to strings
    std::unordered_map<BuildingType, std::string> table;
    table[BuildingType::Farm] = "Farm";
    table[BuildingType::House] = "House";
    table[BuildingType::Lumberjack_Hut] = "Lumberjack Hut";
    table[BuildingType::Quarry] = "Quarry";
    table[BuildingType::Water_Source] = "Water Source";

    return table[b];
}

BuildingType Building::getType() {
    return this->type;
}

const int Building::getID() const {
    return this->id;
}

unsigned int Building::getLevel() {
    return this->level;
}

bool Building::canUpgrade() {
    if(this->level == MAX_LEVEL) {
        return false;
    }
    return true;
}

bool Building::upgrade() {
    if(canUpgrade()) { 
        this->level += 1;
        return true;
    }
    return false;
}

Resource Building::getOutput() {
    if(this->type == BuildingType::Farm) {
        Resource r = Resource(ResourceType::Food, RESOURCE_OUTPUT * this->level);
        return r;
    } else if(this->type == BuildingType::Quarry) {
        Resource r = Resource(ResourceType::Stone, RESOURCE_OUTPUT * this->level);
        return r;
    } else if(this->type == BuildingType::Water_Source) {
        Resource r = Resource(ResourceType::Water, RESOURCE_OUTPUT * this->level);
        return r;
    } else if(this->type == BuildingType::Lumberjack_Hut) {
        Resource r = Resource(ResourceType::Logs, RESOURCE_OUTPUT * this->level);
        return r;
    }
    else {
        CHECK_ERROR
        exit(1);
    }
}
