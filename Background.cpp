#include "Background.h"
#include "Game.h"
#include <fstream>

Background::Background()
{
}

Background::~Background()
{
}

void Background::loadBG(std::string path, int sizeX, int sizeY)
{
	char tile;
	std::fstream mapFile;
	mapFile.open(path);

		
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(tile);
			Game::addTile(atoi(&tile), x * 64, y * 64);
			mapFile.ignore(); // ignore comma or newline char
		}
	}

	mapFile.close();
}
