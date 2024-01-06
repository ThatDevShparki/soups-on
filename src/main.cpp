#include <iostream>
#include "Game.h"
#include "Map.h"

int main()
{
//	Game game("assets/manifest.txt");
//	game.run();


	Map map({ "assets/maps/map_001._map.csv", "assets/maps/map_001._items.csv" });
	map.print();
}
