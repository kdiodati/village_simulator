# Village Builder
#### Kyle Diodati, Carter Perkins, and Taylor Scott 

## Description
Village Builder is a simple base-building game where the player has to manage their own village. They will need to balance resource production and consumption, villager needs, and randomized events. Will your village prosper or fail?

## Running
`make`  
`./village_builder.exe`

## Cleaning
`make clean`

## Documentation
### Structure of the Program (Dependencies and Implementation)

#### Map/Tiles:
Files used for creating the map, implementing tiles, defining resources, etc. Used when first generating the world and accessing Tile properties through the course of the game. Map holds Tile objects that are keys to the buildings built, resources available, etc. Resources and Tiles are implemented separetely.

- resource.hpp
    - resource.cpp
- resource.hpp
- tile.hpp
    - tile.cpp
- resource.hpp
- tile.hpp
- building.hpp
- map.hpp
    - map.cpp

#### Game Menu:
Files used for getting user input and changing the state of tiles. Actions are selected from the state of the Tiles on the Map. Startup menu handles initial game setup and the game menu consolidates the rest of the game actions into the main loop.

- resource.hpp
- tile.hpp
- map.hpp
- building.hpp
- startup_menu.hpp
    - startup_menu.cpp
- game_menu.hpp
    - game_menu.cpp

#### Buildings:
Files used for building definition and what they change/provide. Does not handle placement of buildings (GameMenu.cpp ?)

- resource.hpp
- tile.hpp
- building.hpp
    - building.cpp

#### Villagers:
Villager definition and needs. (Use simple integers at first)

- resource.hpp
- villagers.hpp
    - villagers.cpp

#### Events:
Randomized events in the game. (Separate into abstract files i.e. (WeatherEvents, VillageEvents, etc.))

- resource.hpp
- tile.hpp
- map.hpp
- villagers.hpp
- building.hpp
- events.hpp
    - events.cpp

#### Main Game:
Bringing together the game into a single class to be invoked by the main method (main.cpp). Progression (through a main game loop) and starting conditions handled here.

- resource.hpp
- tile.hpp
- map.hpp
- villagers.hpp
- building.hpp
- game_menu.hpp
- startup_menu.hpp
- game.hpp
    - game.cpp 
