#ifndef BUILDING_HPP_
#define BUILDING_HPP_

#include <string>

#include "tile.hpp"
#include "resource.hpp"


// Building Types
enum class BuildingType {
    House,
    Farm,
    Quarry,
    Lumberjack_Hut,
    Water_Source
};

// For using BuildingType as key for an unordered_map
class BuildingTypeHash {
    public:
        template <typename T>
        std::size_t operator()(T t) const {
            return static_cast<std::size_t>(t);
        }
};


// Building Class 
class Building {
    public:
        Building(BuildingType type_) : level(1), type(type_), id(++uID) {}
        // Getter
        BuildingType getType();
        unsigned int getLevel();
        const int getID() const;
            
        static BuildingType parseBuildingType(std::string str);
        static std::string parseBuildingString(BuildingType t);
	    
        //FIXME having issues with this function
        bool operator==(const Building& other) const {
            return this->id == other.getID();
        }


        // Is the building upgradeable
        bool canUpgrade();
    
        bool upgrade();

        Resource getOutput();

    protected:
        static int uID;

    private:
        unsigned int level;
        BuildingType type;

        int id;
};


// Tile Hash
template<>
class std::hash<Building> {
    public:
        std::size_t operator()(const Building& b) const {
            return std::hash<int>()(b.getID());
        }
};

#endif
