/*
 * Viewer.h
 *
 */ 

#ifndef _VIEWER_H
#define _VIEWER_H

#include <stdlib.h>
#include <glut.h>
#include <IL/ilut.h>


#define MIN_DIST_BETWEEN_POINTS 4.0f



class Viewer {
    
public:
    
    static void init(int w, int h);
    static void draw(void);  
    static void changeSize(int width, int height);
    static void setPerspective();

    void drawBox(float x1, float x2, float y1, float y2, float z); // Draws a box
    void DrawText(GLint x, GLint y, char* s, GLfloat r, GLfloat g, GLfloat b); // Draw text

private:   

    static void setOrthogonal();
    static int  screenWidth;
    static int  screenHeight;


};

#endif
