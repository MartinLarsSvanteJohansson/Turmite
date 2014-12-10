
#include "Map.h"


Map::Map(unsigned int width, unsigned int height)
{
	_width = width;
	_height = height;

	// Initialize map
	_map.resize(_width);
	for( unsigned int i = 0; i < width; i++) {
		_map[i].resize(_height);
		for(unsigned int j = 0; j < height; j++ ) {
		}
	}
}

Map::~Map() 
{

}

unsigned int Map::getWidth() const
{
	return _width;
}

unsigned int Map::getHeight() const
{
	return _height;
}

int Map::getCell(unsigned int x, unsigned int y) const
{
	return _map[x][y];
}

void Map::setCell(int x, int y, int colour)
{
	_map[x][y] = colour;
}