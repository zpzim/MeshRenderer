// Mesh Renderer: main.cpp
//
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <vector>




using namespace std;

const double PI = 3.14159265359;
#include "scene.h"
#include "polygon.h"
//const int WINDOW_WIDTH = 200;
//const int WINDOW_HEIGHT = 200;
const double angle_change = PI * .0625;
//Polygon member function pointer type;
typedef void (Poly::*PolyFn)();
Poly GROUND;
Poly TempPoly;
Poly Main_Poly4;
Poly Main_Poly3;
Poly Main_Poly2;
Poly Main_Poly1;
Poly Main_Poly;


Scene Main_Scene;


 
void MouseEvent(int button, int state, int x, int y)
{
	static PolyFn RFront = &Poly::renderfront;
	static PolyFn RTop = &Poly::rendertop;
	static PolyFn RSide = &Poly::renderside;
	static int click_count = 0;		
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		++click_count;
	if(click_count == 1)
	{
		Main_Scene.Renderer = RFront;
		glutPostRedisplay();
	}
	else if (click_count == 2)
	{
		Main_Scene.Renderer = RTop;
		glutPostRedisplay();
	}
	else
	{
		Main_Scene.Renderer = RSide;
		glutPostRedisplay();
		click_count = 0;
	}

}


void keyPressed (unsigned char key, int x, int y) {
	static int press_count = 1;
	if( key == 'q')
	{
		Main_Scene.reset();
	}
	if( key == 'k' && press_count == 1)
	{
		Main_Scene.objects[1] = Main_Poly;
		++press_count;
	}
	else if( key == 'k' && press_count == 2)
	{
		Main_Scene.objects[1] = Main_Poly1;
		++press_count;
	}
	else if( key == 'k' && press_count == 3)
	{
		Main_Scene.objects[1] = Main_Poly2;
		++press_count;
	}
	else if( key == 'k' && press_count == 4)
	{
		Main_Scene.objects[1] = Main_Poly3;
		press_count = 0;
	}	
	else if( key == 'p')
	{
		if (Main_Scene.PolyInScope != Main_Scene.objects.size() - 1)
			Main_Scene.PolyInScope++;
		else
			Main_Scene.PolyInScope = 1;
	}
	else if( key == 'o')
	{
		if (Main_Scene.PolyInScope != 1)
			Main_Scene.PolyInScope--;
		else
			Main_Scene.PolyInScope = Main_Scene.objects.size() - 1;
	}
	else if( key == 's')
	{
		if(Main_Scene.Rotate_Mode)
			Main_Scene.IncXrot(-angle_change);
		else if(Main_Scene.Translate_Mode)
			Main_Scene.IncYtrans(-5.0);
		else if(Main_Scene.Scale_Mode)
			Main_Scene.IncYscale(-.05);
	}
	else if( key == 'w' )
	{
		if(Main_Scene.Rotate_Mode)
			Main_Scene.IncXrot(angle_change);
		else if(Main_Scene.Translate_Mode)
			Main_Scene.IncYtrans(5.0);
		else if(Main_Scene.Scale_Mode)
			Main_Scene.IncYscale(.05);
			
	}
	else if( key == 'a')
	{
		if(Main_Scene.Rotate_Mode)
			Main_Scene.IncYrot(-angle_change);
		else if(Main_Scene.Translate_Mode)
			Main_Scene.IncXtrans(-5.0);
		else if(Main_Scene.Scale_Mode)
			Main_Scene.IncXscale(-.05);
			
	}
	else if( key == 'd')
	{
		if(Main_Scene.Rotate_Mode)
			Main_Scene.IncYrot(angle_change);
		else if(Main_Scene.Translate_Mode)
			Main_Scene.IncXtrans(5.0);
		else if(Main_Scene.Scale_Mode)
			Main_Scene.IncXscale(.05);
	}
	else if(key == 'z')
	{
		if(Main_Scene.Scale_Mode)
			Main_Scene.IncXYZscale(-.05);
	}
	else if(key == 'x')
	{
		if(Main_Scene.Scale_Mode)
			Main_Scene.IncXYZscale(.05);
	}
	else if (key == 'c')
	{
			if(Main_Scene.Scale_Mode)
				Main_Scene.SetScaleMode(false);	
			else
				Main_Scene.SetScaleMode(true);
	}
	else if( key == 't')
	{
		if(Main_Scene.Translate_Mode)
			Main_Scene.SetTranslateMode(false);
		else
			Main_Scene.SetTranslateMode(true);
	}
	else if( key == 'r')
	{
		if(Main_Scene.Rotate_Mode)
			Main_Scene.SetRotateMode(false);
		else
			Main_Scene.SetRotateMode(true);
	}
	else if( key == 'i')
	{		
		if(Main_Scene.RayTrace_Render_Mode)
			Main_Scene.SetRayTraceMode(false);
		else
			Main_Scene.SetRayTraceMode(true);
	}
	glutPostRedisplay();
}


void GL_Render()
{	
	glClear(GL_COLOR_BUFFER_BIT);
	Main_Scene.renderScene();
	glutSwapBuffers();
}

//Initializes OpenGL attributes
void GLInit(int* argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);


	glutCreateWindow("MeshRenderer");
	glMatrixMode(GL_PROJECTION_MATRIX);
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
	glutDisplayFunc(GL_Render);
}


int main(int argc, char** argv)
{	
	if (argc < 2)
	{
		cerr << "Usage: a.out [inputfilename1 inputfilename2 ....]" << endl;
		exit(0);
	}
	vector<double> L1, L2, M1, M2;
	L1.push_back(1.0);
	L1.push_back(.1);
	L1.push_back(.7);
	L1.push_back(.9);
	L2.push_back(0.0);
	L2.push_back(.1);
	L2.push_back(.7);
	L2.push_back(.9);
	M1.push_back(.3);
	M1.push_back(.4);
	M1.push_back(.6);
	M2.push_back(.1);
	M2.push_back(.2);
	M2.push_back(.3);
	Point3D TMPPT(400, 800, -200);
	Point3D TMPPT2(0, 400,-200);
	Point3D TMPPT3(400, 0, -200);
	Main_Scene.setup();
	//cout << "setup done" << endl;
	for(int i = 1; i < argc; ++i)
	{
		if(i % 4 == 1)
		Main_Scene.addObject(Poly(argv[i]), Material(M1, M1, M2, 200));
		if(i % 4 == 2)
		Main_Scene.addObject(Poly(argv[i]), Material(M1, M2, M1, 200));
		if(i % 4 == 3)
		Main_Scene.addObject(Poly(argv[i]), Material(M2, M2, M1, 200));
		if(i % 4 == 0)
		Main_Scene.addObject(Poly(argv[i]), Material(M1, M1, M1, 200));
	}
	Main_Scene.PolyInScope = 1;
	Main_Scene.lights.push_back(Light(TMPPT, L1, L1, L1));
	Main_Scene.lights.push_back(Light(TMPPT2, L1, L1, L1));
	Main_Scene.lights.push_back(Light(TMPPT3, L1, L1, L1));
	Main_Poly = Poly(argv[1]);
	Main_Poly1 = Main_Poly.subdivide();
	Main_Poly2 = Main_Poly1.subdivide();
	Main_Poly3 = Main_Poly2.subdivide();
	Main_Poly4 = Main_Poly3.subdivide();
	//Main_Scene.addObject(Main_Poly);
	Main_Scene.RayTrace_Render_Mode = false;
	GLInit(&argc, argv);
	glutMouseFunc(MouseEvent);
	glutKeyboardUpFunc(keyPressed);
	glutMainLoop();
	return 0;
}


