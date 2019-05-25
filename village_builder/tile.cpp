#include <string>
#include <iostream>
#include <unordered_map>
#include "tile.hpp"

TileType parseTileType(std::string str) {
    // Table of strings to TileTypes
    std::unordered_map<std::string, TileType> table;
    table["Plains"] = TileType::Plains;
    table["Forest"] = TileType::Forest;
    table["Lake"] = TileType::Lake;
    table["Mountains"] = TileType::Mountains;

    return table[str];
}

std::string parseTileString(TileType t) {
    std::unordered_map<TileType, std::string> table;
    table[TileType::Plains] = "Plains";
    table[TileType::Forest] = "Forest";
    table[TileType::Lake] = "Lake";
    table[TileType::Mountains] = "Mountains";

    return table[t];
}

// Initialize class variable
int Tile::uID = 0;

void Tile::setType(TileType type_) {
    this->type = type_;
}

TileType Tile::getType() {
    return this->type;
}

const int Tile::getID() const {
    return this->id;
}

std::vector<std::string> Tile::display() {
    // Tiles are 5x5

    std::vector<std::string> array;
    switch(this->getType()) {
        case TileType::Lake:
            for(unsigned int i = 0; i < TILE_SIZE; i++) {
                array.push_back(std::string(TILE_SIZE * 2, '~'));
            }
            break;
        case TileType::Plains:
            for(unsigned int i = 0; i < TILE_SIZE; i++) {
                array.push_back(std::string(TILE_SIZE * 2, '='));
            }
            break;
        case TileType::Mountains:
            for(unsigned int i = 0; i < TILE_SIZE; i++) {
                array.push_back(std::string(TILE_SIZE * 2, '^'));
            }
            break;
        case TileType::Forest:
            for(unsigned int i = 0; i < TILE_SIZE; i++) {
                array.push_back(std::string(TILE_SIZE * 2, 'T'));
            }
            break;
    }
    return array;
}

