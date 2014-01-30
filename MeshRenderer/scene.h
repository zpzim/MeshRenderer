#ifndef S_CE_NE_H
#define S_CE_NE_H


#include<vector>
#include "polygon.h"
#include "light.h"



using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
typedef void (Poly::*PolyFn)();
const PolyFn DefaultRenderer = &Poly::renderfront;


struct Scene
{
	int PolyInScope;
	PolyFn Renderer;
	bool RayTrace_Render_Mode;
	bool Rotate_Mode;
	bool Translate_Mode;
	bool Scale_Mode;
	void SetScaleMode(bool val);
	void SetRotateMode(bool val);
	void SetTranslateMode(bool val);
	void SetRayTraceMode(bool val);
	vector<Poly> objects;
	vector<double> Xrotations;
	vector<double> Yrotations;
	vector<double> Xtranslations;
	vector<double> Ytranslations;
	vector<double> Xscales;
	vector<double> Yscales;
	vector<double> XYZscales;
	vector<Light> lights;
	vector<Material> materials;
	Scene() : PolyInScope(NULL), Renderer(DefaultRenderer), Xscales(),Yscales(), XYZscales(), lights(), objects(), materials(), RayTrace_Render_Mode(false), Rotate_Mode(false), Translate_Mode(false), Scale_Mode(false) {}
	Scene(vector<Poly> polys, vector<Light> lighting, PolyFn render_view) : PolyInScope(0), Renderer(render_view), lights(lighting), objects(polys), RayTrace_Render_Mode(false), Rotate_Mode(false), Translate_Mode(false), Scale_Mode(false) {}
	void renderScene();
	void addObject(const Poly p, const Material m ) { objects.push_back(p); Xscales.push_back(1), Yscales.push_back(1), XYZscales.push_back(1), Xrotations.push_back(0.0); Yrotations.push_back(0.0); Xtranslations.push_back(0.0); Ytranslations.push_back(0.0); materials.push_back(m); }
	void addLight(const Light L) { lights.push_back(L); }
	void SetPolyInScope(int polynum) {PolyInScope = polynum;}
	void SetXrot(double amt);
	void IncXrot(double amt);  
	void SetYrot(double amt); 
	void IncYrot(double amt);  
	void SetXtrans(double amt); 
	void IncXtrans(double amt);  
	void SetYtrans(double amt); 
	void IncYtrans(double amt); 
	void IncYscale(double amt);
	void IncXscale(double amt);
	void IncXYZscale(double amt);
	void reset();
	void setup();
};

void Scene::SetScaleMode(bool val)
{
	if( val == true)
	{
		if(!Scale_Mode)
		{
			Scale_Mode = true;
			Rotate_Mode = false;
			Translate_Mode = false;
			RayTrace_Render_Mode = false;
		}
	}
	else
		Scale_Mode = false;


}

void Scene::SetRotateMode(bool val)
{
	if (val == true)
	{
		if(!Rotate_Mode)
		{
			Rotate_Mode = true;
			Translate_Mode = false;
			Scale_Mode = false;
			RayTrace_Render_Mode = false;
		}
	}
	else
		Rotate_Mode = false;
}
void Scene::SetTranslateMode(bool val)
{
	if (val == true)
	{
		if(!Translate_Mode)
		{
			Translate_Mode = true;
			Rotate_Mode = false;
			Scale_Mode = false;
			RayTrace_Render_Mode = false;
		}
	}
	else
		Translate_Mode = false;

}

void Scene::SetRayTraceMode(bool val)
{
	if(val == true)
	{
		if(!RayTrace_Render_Mode)
		{
			RayTrace_Render_Mode = true;
			Rotate_Mode = false;
			Translate_Mode = false;
			Scale_Mode = false;
		}
	}
	else
		RayTrace_Render_Mode = false;
}
void Scene::IncYscale(double amt)
{
	if(Scale_Mode)
		Yscales[PolyInScope] += amt;

}

void Scene::IncXscale(double amt)
{
	if(Scale_Mode)
		Xscales[PolyInScope] += amt;

}

void Scene::IncXYZscale(double amt)
{
	if(Scale_Mode)
		XYZscales[PolyInScope] += amt;

}


void Scene::SetXrot(double amt)
{
	if(Rotate_Mode)
		Xrotations[PolyInScope] = amt;}
	
void Scene::IncXrot(double amt)
{
	if(Rotate_Mode)
		Xrotations[PolyInScope] += amt;
}

void Scene::SetYrot(double amt)
{
	if(Rotate_Mode)
		Yrotations[PolyInScope] = amt;
}

void Scene::IncYrot(double amt)
{
	if(Rotate_Mode)
		Yrotations[PolyInScope] += amt;
}

void Scene::SetXtrans(double amt)
{
	if(Translate_Mode)
		Xtranslations[PolyInScope] = amt;
}
void Scene::IncXtrans(double amt)
{
	if(Translate_Mode)
		Xtranslations[PolyInScope] += amt;
}

void Scene::SetYtrans(double amt)
{
	if(Translate_Mode)
		Ytranslations[PolyInScope] = amt;
}
	
void Scene::IncYtrans(double amt)  
{
	if(Translate_Mode)
		Ytranslations[PolyInScope] += amt;
}

#include "raytrace.h"
void Scene::renderScene()
{
	Poly TempPoly;
	Scene Temp(vector<Poly>(), this -> lights, this -> Renderer);
	Temp.PolyInScope = this -> PolyInScope;
	Temp.Rotate_Mode = this -> Rotate_Mode;
	Temp.RayTrace_Render_Mode = this -> RayTrace_Render_Mode;
	Temp.Translate_Mode = this -> Translate_Mode;
	Temp.Xrotations = this -> Xrotations;
	Temp.Xtranslations = this -> Xtranslations;
	Temp.Yrotations = this -> Yrotations;
	Temp.Ytranslations = this -> Ytranslations;
	for(int i = 0; i < Xrotations.size(); ++i)
	{
		TempPoly = objects[i].XYscale(Xscales[i], Yscales[i]);
		TempPoly = TempPoly.XYZscale(XYZscales[i]);
		TempPoly = TempPoly.rotatetheta(Xrotations[i]);
		TempPoly = TempPoly.rotatephi(Yrotations[i]);
		TempPoly = TempPoly.translate(Xtranslations[i], Ytranslations[i], 0.0);
		Temp.addObject(TempPoly, materials[i]);
	}
	if(!RayTrace_Render_Mode)
	{
		for( int i = 0; i < Temp.objects.size(); ++i)
		{
			((Temp.objects[i]).*Renderer)();
		}
	}
	else
	{
			RayTrace(WINDOW_HEIGHT, WINDOW_WIDTH, Temp);
	}
}



void Scene::setup()
{
	static bool first = true;
	static Poly GROUND;
	
	static Material GROUND_MAT;
	if(first)
	{
		GROUND.numVertices = 4;
		GROUND.numFaces = 2;
		GROUND.verts.push_back(make_pair(Vertex(Point3D(0, 0, 1000)), vector<Face >()));
		GROUND.verts.push_back(make_pair(Vertex(Point3D(800, 0, 1000)), vector<Face >()));
		GROUND.verts.push_back(make_pair(Vertex(Point3D(0, 800, 1000)), vector<Face >()));
		GROUND.verts.push_back(make_pair(Vertex(Point3D(800, 800, 1000)), vector<Face >()));
		GROUND.poly.push_back(Face(0, 1, 2));
		GROUND.poly.push_back(Face(3, 2, 1));
		this -> addObject(GROUND, GROUND_MAT);
		first = false;
	}
}
void Scene::reset()
{
		XYZscales.assign(objects.size(), 1.0);
		Yscales.assign(objects.size(), 1.0);
		Xscales.assign(objects.size(), 1.0);
		Xrotations.assign(objects.size(), 0.0);
		Yrotations.assign(objects.size(), 0.0);
		Xtranslations.assign(objects.size(), 0.0);
		Ytranslations.assign(objects.size(), 0.0);
		PolyInScope = 1;
		RayTrace_Render_Mode = false;
		Renderer = DefaultRenderer;
}



#endif
