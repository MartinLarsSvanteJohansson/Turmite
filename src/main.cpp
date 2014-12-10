/*
 *
 * This program implements a Relative Turmite on a rectangular map. 
 *
 * Implemented functionalities:
 *		Langton's ant
 *			R and L turning directions
 *			Multiple colours
 *		Boundary conditions
 *			Fixed
 *			Toroidal
 *		Multiple states
 *			2state - 2colours
 *
 *	
 * -- USER INPUTS --
 *
 * There are three input files that you can modify.
 *
 * 1) settings.txt 
 * Here you can specify the width and height grid size on the first line and the boundary condition
 * on the second line. A valu of 0 = fixed boundary, and 1 = toroidal
 * Example:
 * 150 150
 * 1
 * 
 * 2) input_langton.txt
 * Enter the rotational rules for each colour with R for right turn and L for left turn. 
 * The Number of rotational rules set will also be the number of implemented colours
 * Example:
 *		RL
 * 		LRLRLRLRLRLRLRL
 *
 * 3) input_turmite.txt
 * Here you can specify the 12 digits corresponding to a 2-state 2-colour turmite. 
 * The machines are encoded  to the form {a, b, c} where a=new colour, b=movement, c=new state.
 * The movement is specified according to the literature:
 * 		1: no turn; 2: right; 4: u-turn; 8: left
 * 
 * Examples:
 *
 * Fibonacci {1,8,1}, {1,8,1}, {1,2,1}, {0,1,0}. 
 *	181181121010
 * Spiral:
 *	111180121010	
 * 
 *             
   *     ?[-?_•?]?
 * Author: Martin Lars Svante Johansson  
 * email: martin.lars.svante.johansson@gmail.com
 * 
 *
 */

#include "Turmite.h"
#include "Viewer.h"

#include <iostream>
#include <sstream>
#include <IL/ilut.h>



static int winWidth     = 1324;
static int winHeight    = 768;

// Global interface classes
std::unique_ptr<Turmite>	turmite;
std::unique_ptr<Viewer>		viewer;

// -----------------------

void timer(int time)
{
	turmite->setTime(time);
	glutPostRedisplay();
}

// Timer callback function
void timerCallback(int temp) {
	if(!turmite->pause) {
		turmite->run();
		glutTimerFunc(turmite->getTime(), timerCallback, 0);
	}
	glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y) 
{
	switch (key) {
		case 27: // Quit
			exit(0);
			break;

		case 's': // Start
		case 'i':
		case 'S':
		case 'I':
			glutPostRedisplay();
			turmite->setDrawFunction(0);
			turmite->pause=false;
			turmite->run();
			glutTimerFunc(turmite->getTime(), timerCallback, 0);
			break;

		case 'm': // ´Draw the 2 state
			glutPostRedisplay();
			turmite->setDrawFunction(1);			
			turmite->pause=false;
			turmite->run();
			glutTimerFunc(turmite->getTime(), timerCallback, 0);
			break;

		case 'p': // Pause/Restart
		case 'P':
			turmite->pause=true;
			break ;

		case 'r': // Re-initialize
			turmite->reinitialize();
			glutPostRedisplay();
			break;
		default:
			break ;
	}
}

// Window adjustments
void changeSize(int width, int height)
{
	winWidth        = width;
    winHeight       = height;    		

	// Set perspective of viewer
	viewer->setPerspective();

	glutPostRedisplay();
}



void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef(0.0, 0.0, -25.0);

	unsigned int width	= turmite->getWidth();
	unsigned int height = turmite->getHeight();

	double scale = 0.9;
	float z = turmite->getDepth();

	// Draw map
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(turmite->getCell(i,j) != 0) {
				
				// Set map cell colour
				if(turmite->getCell(i,j) == 1)
					glColor3f(0.2, 0.2, 0.2);
				else {
					float maxC = turmite->getMaxColours();
					float c = turmite->getCell(i,j);
					float r = c/maxC;
					float g = 1- c/maxC;
					float b = c/maxC;
					glColor3f(r, g, b);
				}

				// Draw a map cell
				float x1 = -(width/2+0.5)*scale + scale*i;
				float x2 = -(width/2+0.5)*scale + scale* (i + 1);
				float y1 = (height/2-0.5)*scale - scale* j;
				float y2 = (height/2-0.5)*scale - scale* (j - 1);

				viewer->drawBox(x1, x2, y1, y2, z);
			}
		}
	}

	// Draw the map border
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(4.0f);
	glBegin(GL_LINES);

		float offset = -7;
		float x1 = -(width/2+0.5 + offset);
		float x2 = (width/2-0.5 + offset);
		float y1 = -(height/2-0.5 + offset);
		float y2 = (height/2+0.5 + offset);

		glVertex3f(x1, y1, z);
		glVertex3f(x2, y1, z);

		glVertex3f(x2, y1, z);
		glVertex3f(x2, y2, z);

		glVertex3f(x2, y2, z);
		glVertex3f(x1, y2, z);

		glVertex3f(x1, y2, z);
		glVertex3f(x1, y1, z);

	glEnd();
	
	// Draw some information text
	char *information = new char[50];

	std::string information2;
	
	

	sprintf_s(information,50,"Steps: %d ", turmite->counter);
	viewer->DrawText(-26,-0.8,information, 0.2f, 0.2f, 0.2f);
	sprintf_s(information,50,"Map Size: %dx%d",turmite->getWidth(),turmite->getHeight());
	viewer->DrawText(-26,-2,information, 0.2f, 0.2f, 0.2f);

	if(turmite->toroidalWalk)
		sprintf_s(information,50,"Boundary: Toroidal ");
	else
		sprintf_s(information,50,"Boundary: Bounded ");
	viewer->DrawText(-26,-4,information, 0.2f, 0.2f, 0.2f);

	int scaleI = ((turmite->getWidth())-110)*0.04;
	
	sprintf_s(information,50,"Press S to start");
	viewer->DrawText(16+scaleI,8.0,information, 0.1f, 0.1f, 0.1f);
	sprintf_s(information,50,"Press M to start a multistate");
	viewer->DrawText(16+scaleI,6.0,information, 0.1f, 0.1f, 0.1f);
	sprintf_s(information,50,"Press P to pause");
	viewer->DrawText(16+scaleI,4.0,information, 0.1f, 0.1f, 0.1f);
	sprintf_s(information,50,"Press R to reinitialize");
	viewer->DrawText(16+scaleI,2.0,information, 0.1f, 0.1f, 0.1f);
	sprintf_s(information,50,"Press I to increase speed");
	viewer->DrawText(16+scaleI,0.0,information, 0.1f, 0.1f, 0.1f);
	sprintf_s(information,50,"Press Esc to quit");
	viewer->DrawText(16+scaleI,-2.0,information, 0.1f, 0.1f, 0.1f);
	sprintf_s(information,50,"Right click to set speed");
	viewer->DrawText(16+scaleI,-4.0,information, 0.1f, 0.1f, 0.1f);

	glutSwapBuffers();
}

/* Program entry point */
int 
main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    glutInitWindowSize(winWidth,winHeight);
    glutInitWindowPosition(300,50);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    glutCreateWindow("Turmites v01    ----     Martin Johansson");

	// Parse settings file
	std::ifstream file_settings("settings.txt");
	if(file_settings.is_open()) {
		std::cout << "Successfully opened settings.txt" << std::endl;
	}
	if( !file_settings.is_open() ) {
		file_settings.open("../settings.txt");
	}
	if(file_settings.is_open()) {
		std::cout << "Successfully opened settings.txt in down folder" << std::endl;
	}

	std::string str;
	std::vector<int> v;

	// Debug
	
	// Get map size and boarder settings
	while(std::getline(file_settings, str)) {
		std::istringstream iss(str);
		int n;	
		while (iss >> n)
			v.push_back(n);
	}

	// Validate input
	if(v.size() != 3 || v[0] < 2 || v[1] < 2 || v[2]<0 || v[2]>1) {
		printf("Error reading settings file");
		return EXIT_FAILURE;	
	}
	
	// Create the turmite on the specified map size
	turmite.reset(new Turmite(v[0], v[1]));
	
	// Set boundary condition
	turmite->toroidalWalk = v[2];
	
	bool success = turmite->readInput();
	if(!success) {
		printf("Error loading input file.");
		return EXIT_FAILURE;
	}

	//callbacks
	glutDisplayFunc(draw);
	glutReshapeFunc(changeSize);
	glutCreateMenu(timer);
		glutAddMenuEntry("Fastest",   1);
		glutAddMenuEntry("2 ms delay",   2);
		glutAddMenuEntry("10 ms delay",   10);
		glutAddMenuEntry("100 ms delay",   100);
		glutAddMenuEntry("Slowest",  1000);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	 // Setup keyboard
	glutKeyboardFunc(keyboard);

    ilInit();  
    iluInit();      
    ilutRenderer(ILUT_OPENGL);                  		

	Viewer::init(winWidth,winHeight);

    glutMainLoop();


    return EXIT_SUCCESS;
}



