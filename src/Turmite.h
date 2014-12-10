/*
 * Turmite.h
 *
 */ 

#ifndef _TURMITE_H
#define _TURMITE_H


#include "Map.h"

#include <memory>
#include <fstream>
#include <string>
#include <glut.h>



class Turmite {
public:
	Turmite(unsigned int width, unsigned int height);
	~Turmite();

	// Reset the turmite
	void reinitialize();

	
	int     getCell() const; // Get cell colour for turmite location
	int     getCell(int x, int y) const; // Get a specified cell colour
	void    setCell(int colour); // Set sell colour for turmite location
	int     getX() const;
	int     getY() const;
	int     getWidth() const;
	int     getHeight() const;
	float   getDepth() const;
	int     getMaxColours() const;
	void    setMaxColours(int max);
	int     getState() const;
	void    setState(int state); // Set wich function to draw, Langton=0; multistate=2
	void    setDrawFunction(int function);
	bool    readInput();
	void    run(); // The main algorithm steps
	int     getTime() const;
	void    setTime(int time);


	bool	pause;
	bool	toroidalWalk; // If we are walking on a toroid
	int	counter; // Step counter

	
	
private:
	
	unsigned int            _x; // Turmite map locations
	unsigned int            _y;
	std::unique_ptr<Map>    _map;
	float                   _depth; // Painting depth	
	int                     _dir; // Turmite direction. 0: UP; 1: RIGHT; 2: DOWN; 3: LEFT
	bool                    _multistate; // If multistate machine is on
	int                     _state; // Curent state. 
	int                     _machine[12]; // 2 state machine vector
	int                     _maxColours; // Maximum colours
	std::vector<int>        _colourRule; // Colour rule from input. 1=R, 0=L
	unsigned int            _time; // Drawing time in ms

	bool        mapCheck() const; // Check to se if we are within the map borders
	int         update_langton(); // Update the current cell colour. Returns the rotation direction (CW / CCW)
	int         update_multistate();
	void        turn(int rotation); // Turn clockwise(rotation=1) or counter-clockwise(rotation=0), or no turn(rotation=-1)
	void        move(); // Take one step forward in current direction

};

#endif