#ifndef TILE_HPP_
#define TILE_HPP_

#include <vector>
#include <string>

#define TILE_SIZE 10

// Tile Types
enum class TileType {
    Mountains,
    Plains,
    Forest,
    Lake  
};


// Parse string to TileType
TileType parseTileType(std::string);
std::string parseTileString(TileType t);

// Tile Class
class Tile {
    public:
        // Constructor
        Tile(TileType type_) : type(type_), id(++uID) {}


        // Display for printing
        std::vector<std::string> display();

        // Getter and Setter for TileType
        void setType(TileType type_);
        TileType getType();
       
        // Get the ID of the instance
        const int getID() const;

        bool operator==(const Tile& other) const {
            return this->getID() == other.getID();
        }
            
    protected:
        static int uID;
    
    private:
        // Type of Tile
        TileType type;
        
        // ID of the instance
        const int id;
};

// Tile Hash
template<>
class std::hash<Tile> {
    public:
        std::size_t operator()(const Tile& t) const {
            return std::hash<int>()(t.getID());
        }
};

#endif
