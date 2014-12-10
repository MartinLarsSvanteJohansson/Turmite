
#include "Turmite.h"

Turmite::Turmite(unsigned int width, unsigned int height)
{
	_map.reset(new Map(width, height));
	
	// Set Turmite origin
	_x = width/2;
	_y = height/2;
	_dir = 0;
	_depth = -(std::max(width,height)*0.6f);
	setState(0);
	
	toroidalWalk = true;
	counter = 0;
	_time = 5;
	pause = false;
	_maxColours = 0;
	_multistate = false;

	for( unsigned int i = 0; i < _map->getWidth(); i++) {
		for(unsigned int j = 0; j < _map->getHeight(); j++ ) {
			_map->setCell(i, j, 0);
		}
	}
}

bool Turmite::readInput()
{

	// Parse input_langton.txt
	// ----------------------------------------------
	std::ifstream file_langton("input_langton.txt");
	std::string str;

	if (!file_langton.is_open())
		return false;

	while(std::getline(file_langton, str)) {

	}

	file_langton.close();

	printf("\nInput Colour Rule: ");
	for( auto it = str.begin(); it !=str.end(); it++ ) {

		_maxColours++;
		
		if(*(it) == 'R' || *(it) == 'r') {
			_colourRule.push_back(1);
			printf("R");
		}
		else if(*(it) == 'L' || *(it) == 'l') {
			_colourRule.push_back(0);
			printf("L");
		}
		else
			return false;
	}
	// -------------------------------------------------

	// Parse input_turmite.txt
	// -------------------------------------------------
	
	std::ifstream file_turmite("input_turmite.txt");
	if (!file_turmite.is_open())
		return false;

	std::string str2;
	std::getline(file_turmite, str2);

	int i=0;
	for (auto it2 = str2.begin(); it2 !=str2.end(); it2++ ) {
		
		// Convert input to integer and store in the array
		_machine[i] = static_cast<int>(str2[i]) - 48;

		// If turning state
		if(i ==1 || i==4 || i==7 || i==10) {
			// Order the rotations
			if(_machine[i] == 8)
				_machine[i] = 0;
			else if(_machine[i] == 2)
				_machine[i] = 1;
			else if(_machine[i] == 1)
				_machine[i] = -1;
		}
		i++;
	}

	return true;
}

void Turmite::reinitialize()
{
	_x = _map->getWidth()/2;
	_y = _map->getHeight()/2;
	_dir = 0;
	counter = 0;
	_depth = -(std::max(getWidth(),getHeight())*0.6f);

	for( unsigned int i = 0; i < _map->getWidth(); i++) 
		for(unsigned int j = 0; j < _map->getHeight(); j++ )
			_map->setCell(i, j, 0);	
}

int Turmite::getX() const 
{
    return _x;
}

int Turmite::getY() const 
{
    return _y;
}

int Turmite::getCell() const 
{
    return _map->getCell(_x, _y);
}

int Turmite::getCell(int x, int y) const 
{
	return _map->getCell(x, y);
}

void Turmite::setCell(int colour) 
{
	_map->setCell(_x, _y, colour);
}

int Turmite::getWidth() const 
{
	return _map->getWidth();
}

int Turmite::getHeight() const 
{
	return _map->getHeight();
}

float Turmite::getDepth() const
{
	return _depth;
}

int Turmite::getMaxColours() const 
{
	return _maxColours;
}

void Turmite::setMaxColours(int max)  
{
	_maxColours = max;
}

int Turmite::getState() const 
{
	return _state;
}

void Turmite::setState(int state)  
{
	_state = state;
}

// Langton = 0; Fibonacci = 1;
void Turmite::setDrawFunction( int function)  
{
	if(function == 0 && _multistate) {
		reinitialize();
		_multistate = false;
		setMaxColours(_colourRule.size());
	}
	// Fibonacci
	else if (function == 1 && !_multistate){

		// Machine states
		reinitialize();
		_multistate = true;
		setMaxColours(2);
	}
}

// Returns true if we are within the map
bool Turmite::mapCheck() const
{
	bool test = _x >= 0 &&  _y >= 0 && _x < _map->getWidth() && _y < _map->getHeight();
	return test;
}

int Turmite::getTime() const
{
	return _time;
}

void Turmite::setTime(int time)
{
	_time = time;
}

void Turmite::run()
{
	// Check border collision
	bool success = mapCheck();

	if(success) {

		// In what direction should we rotate, if any
		int rotating;

		// Update the cell colour and turmite state
		if(_multistate) {
			rotating = update_multistate();
		}
		// Update the cell colour according to langton's ant
		else {
			rotating = update_langton();
		}

		// Turn
		turn(rotating);

		// Move one step
		move();
	}
	else {
	}

	counter++;
	glutPostRedisplay();
}

int Turmite::update_multistate()
{
	int rotating;

	// 181 181 121 010
	if(getState() == 0) {
		if(getCell() == 0) {
			setCell(_machine[0]); 
			rotating = _machine[1];
			setState(_machine[2]);
		}
		else if (getCell() == 1) {
			setCell(_machine[3]);
			rotating = _machine[4];
			setState(_machine[5]);
		}
	}
	else if (getState() == 1 ) {
		if(getCell() == 0) {
			setCell(_machine[6]);
			rotating = _machine[7];
			setState(_machine[8]);
		}
		else if (getCell() == 1) {
			setCell(_machine[9]);
			rotating = _machine[10];
			setState(_machine[11]);
		}
	}

	// Return the rotation direction
	return rotating;
}

int Turmite::update_langton()
{
	// Get the current cell colour
	int colour = getCell();

	// Increase the cell colour
	setCell((colour +1) % _maxColours);

	// Clockwise or Counterclockwise ?
	if(_colourRule[colour] == 1)
		return 1; 
	else if(_colourRule[colour] == 0)
		return 0;

	return -1;
}

void Turmite::turn(int rotation)
{
	if(rotation == 1) // If 2
		_dir = (_dir+1) % 4;
	else if(rotation == 0) // if 8
		_dir = (_dir+3) % 4;
}

void Turmite::move()
{
	// Reversed y-axis needed
	if(_dir == 0)
		_y--;
	else if(_dir == 1)
		_x++;
	else if(_dir == 2)
		_y++;
	else if(_dir == 3)
		_x--;

	if(toroidalWalk) {
		if(mapCheck() == false) {			
			_x = (_x + _map->getWidth()) % _map->getWidth();
			_y = (_y + _map->getHeight()) % _map->getHeight();
		}
	}
}

Turmite::~Turmite()
{
	
}