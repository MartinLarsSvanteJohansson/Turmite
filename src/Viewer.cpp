/*
 *
 *
 */ 

#include "Viewer.h"

// Static member declarations
int Viewer::screenWidth;
int Viewer::screenHeight;


void Viewer::init(int w, int h) 
{   	
    screenWidth=w;
    screenHeight=h;  	

	// Set clear/active colours
	glClearColor(1,1,1,1);
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

void Viewer::setPerspective() 
{
	// Set perspective to current screen width/height
	glViewport(0, 0, (GLfloat)glutGet(GLUT_WINDOW_WIDTH), (GLfloat)glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0,(GLfloat)glutGet(GLUT_WINDOW_WIDTH)/(GLfloat)glutGet(GLUT_WINDOW_HEIGHT),0.001,1000.0f);    
    
	glMatrixMode(GL_MODELVIEW);
}

void Viewer::setOrthogonal() 
{
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 
	    0.0, glutGet(GLUT_WINDOW_HEIGHT), -10.0, 10.0);	

	glMatrixMode(GL_MODELVIEW);
}

void Viewer::DrawText(GLint x, GLint y, char* s, GLfloat r, GLfloat g, GLfloat b)
{
	int lines;
	char* p;

	glDisable(GL_DEPTH_TEST);
	glPushMatrix();	

	glColor4f(r,g,b,1.0);
	glRasterPos2i(x, y);

	for(p = s, lines = 0; *p; p++) {		
		
		if (*p == '\n') {
			lines++;
			glRasterPos2i(x, y-(lines*18));
		}
		
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
	}
	
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Viewer::drawBox(float x1, float x2, float y1, float y2, float z)
{
	glBegin(GL_QUADS);
		glVertex3f(x1, y1, z);
		glVertex3f(x2, y1, z);
		glVertex3f(x2, y2, z);
		glVertex3f(x1, y2, z);
	glEnd();
}

void Viewer::changeSize(int width, int height) {

	screenWidth		= width;
    screenHeight	= height;    		

	// Reset the perspective
	setPerspective();		

	// Redisplay normal plane
	glutPostRedisplay();

}


