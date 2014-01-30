#ifndef __RENDER_ING_H_
#define __RENDER_ING_H_

#include<GL/glut.h>
#include<algorithm>

using namespace std;

// Renders a quad at cell (x, y) with dimensions CELL_LENGTH
void renderPixel(int x, int y, float r = 1.0, float g = 1.0, float b = 1.0)
{
	glBegin(GL_POINTS);
	glColor3f(r,g,b);
	glVertex2i(x,y);
	glColor3f(1,1,1);
	glEnd();
}



void renderLine(int p1x, int p1y, int p2x, int p2y)
{
     bool isSteep = abs(p2y - p1y) > abs(p2x - p1x);
     if (isSteep)
     {
	 swap(p1x, p1y);
         swap(p2x, p2y);
     }
     if (p1x > p2x)
     { 
	 swap(p1x, p2x);
         swap(p1y, p2y);
     }
     int dx = p2x - p1x;
     int dy = abs(p2y - p1y);
     int error = dx / 2;
     int ystep;
     int y = p1y;
     if (p1y < p2y)
         ystep = 1;
     else 
	 ystep = -1;
     for(int x = p1x; x < p2x; ++x)
     {
	 if (isSteep)
		renderPixel(y,x);
	 else
		renderPixel(x,y);
         error -= dy;
         if (error < 0)
         {
	     y = y + ystep;
             error += dx;
	 }
     }
}
#endif
