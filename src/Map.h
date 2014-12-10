/*
 * Map.h
 *
 */ 

#ifndef _MAP_H
#define _MAP_H

#include <vector>
#include <algorithm>


// The Map Class
class Map {
public:

	Map(unsigned int width, unsigned int height);
	~Map();

	unsigned int    getWidth() const;
	unsigned int    getHeight() const;
	int             getCell(unsigned int x, unsigned int y) const; // Get the colour value for the given cell
	void            setCell(int x, int y, int colour); // Set the colour value 

protected:
	
	std::vector<std::vector<int> >  _map; // Simple map
	unsigned int                    _width;
	unsigned int                    _height;
};

#endif