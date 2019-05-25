FLAGS=-g -c -std=c++11
FLAGSEXE=-std=c++11 -o

village_builder.exe : main.o game.o startup_menu.o game_menu.o \
	map.o building.o \
	villagers.o tile.o resource.o events.o
		g++ $(FLAGSEXE) village_builder.exe main.o game.o startup_menu.o game_menu.o \
			map.o building.o villagers.o tile.o resource.o events.o

main.o : main.cpp game.hpp
		g++ $(FLAGS) main.cpp
game.o : game.cpp game.hpp
		g++ $(FLAGS) game.cpp
startup_menu.o : startup_menu.cpp startup_menu.hpp
		g++ $(FLAGS) startup_menu.cpp
game_menu.o : game_menu.cpp game_menu.hpp
		g++ $(FLAGS) game_menu.cpp
map.o : map.cpp map.hpp
		g++ $(FLAGS) map.cpp
building.o : building.cpp building.hpp
		g++ $(FLAGS) building.cpp
villagers.o : villagers.cpp villagers.hpp
		g++ $(FLAGS) villagers.cpp
tile.o : tile.cpp tile.hpp
		g++ $(FLAGS) tile.cpp
resource.o : resource.cpp resource.hpp
		g++ $(FLAGS) resource.cpp
events.o : events.cpp events.hpp
		g++ $(FLAGS) events.cpp


clean :
		rm village_builder.exe main.o game.o startup_menu.o game_menu.o \
		map.o building.o villagers.o tile.o resource.o events.o
