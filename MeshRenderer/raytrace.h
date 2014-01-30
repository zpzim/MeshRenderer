#ifndef __RAY_TRACE____H
#define __RAY_TRACE____H

#include "point3d.h"
#include "polygon.h"
#include "rendering.h"
#include "light.h"
#include "scene.h"
#include "vectorray.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>



double Determinant( const double &a, const double &b, const double &c, const double &d, const double &e, const double &f, const double &g, const double &h, const double &i  )
{
	return (a * e * i + b * f * g + c * d * h) - (c * e * g + b * d * i + a * f * h);
}


bool castRay(Ray r, const Scene& scene, pair<Point3D, pair<int, int> > &result )
{
	double A, B, C, D, E, F, G, H, I, J, K, L;
	vector<pair < pair <int, int >, double> > resulttemp;
	int i, j;
	for(i = 0; i < scene.objects.size(); ++i)
	{
			for(j = 0; j < scene.objects[i].poly.size(); ++j)
			{

	
					Point3D a, b, c, Eb, Ec;
					double DetA, DetB, DetC, DetT;
					a = scene.objects[i].verts[scene.objects[i].poly[j].v1].first.v;
					b = scene.objects[i].verts[scene.objects[i].poly[j].v2].first.v;
					c = scene.objects[i].verts[scene.objects[i].poly[j].v3].first.v;
					Eb = b - a;
					Ec = c - a;
					A = -Eb.x;//f
					B = -Eb.y;
					C = -Eb.z;
					D = (-Ec.x);//g
					E = (-Ec.y);
					F = (-Ec.z);
					G = (r.Rd.x);//h
					H = (r.Rd.y);
					I = (r.Rd.z);
					DetA = Determinant(A, D, G, B, E, H, C, F, I);
					if (DetA == 0)
						continue;
					J = (a.x - r.Ro.x); //k
					K = (a.y - r.Ro.y);
					L = (a.z - r.Ro.z);
					DetT = Determinant(A, D, J, B, E ,K, C , F, L);
					double t = DetT / (double) DetA;
					if (t <= 0)
						continue;
					DetB = Determinant(J, D, G, K, E, H,  L, F, I);
					double beta = DetB / (double) DetA;
					if (beta <= 0)
						continue;
					DetC = Determinant(A, J, G, B, K, H, C, L, I);
					double gamma = DetC / (double) DetA;
					if (gamma <= 0)
						continue;
					if (beta + gamma >= 1)
						continue;

					resulttemp.push_back(make_pair(make_pair(i, j), t));
			}
	}
	if(resulttemp.size() == 0)
		return false;
	double lowZ = (r.Ro +  r.Rd * resulttemp[0].second).z;
	int minZidx = 0;
	for(int v = 1; v < resulttemp.size(); ++v)
	{
		double curZ = (r.Ro +  r.Rd * resulttemp[v].second).z;
		if (curZ < lowZ)
		{			
			minZidx = v;
			lowZ = curZ;
		}
	}
	result = make_pair(r.Ro +  r.Rd * resulttemp[minZidx].second, make_pair(resulttemp[minZidx].first.first, resulttemp[minZidx].first.second));
	return true;
		
		
}

bool castShadowRay(const int polynum, const int facenum, const Point3D &p, Ray r, const Scene &scene, Vector3D &result)
{
	Point3D centroid;
	//bool first_intersect = true;
	double A, B, C, D, E, F, G, H, I, J, K, L;
	//for(int i = 0; i < scene.objects.size(); ++i)
	//{
			//for(int j = 0; j < scene.objects[i].poly.size(); ++j)
			//{
					Point3D a, b, c, Eb, Ec;
					double DetA, DetB, DetC, DetT;
					a = scene.objects[polynum].verts[scene.objects[polynum].poly[facenum].v1].first.v;
					b = scene.objects[polynum].verts[scene.objects[polynum].poly[facenum].v2].first.v;
					c = scene.objects[polynum].verts[scene.objects[polynum].poly[facenum].v3].first.v;
					if(polynum != 0)
						centroid = scene.objects[polynum].pointAvg();
					else
						centroid = Point3D(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1000);
					Eb = b - a;
					Ec = c - a;
					A = -Eb.x;//f
					B = -Eb.y;
					C = -Eb.z;
					D = (-Ec.x);//g
					E = (-Ec.y);
					F = (-Ec.z);
					G = (r.Rd.x);//h
					H = (r.Rd.y);
					I = (r.Rd.z);
					DetA = Determinant(A, D, G, B, E, H, C, F, I);
					if (DetA == 0)
						return false;
					J = (a.x - r.Ro.x); //k
					K = (a.y - r.Ro.y);
					L = (a.z - r.Ro.z);
					DetT = Determinant(A, D, J, B, E ,K, C , F, L);
					double t = DetT / (double) DetA;
					if (t <= 0)
						return false;
					DetB = Determinant(J, D, G, K, E, H,  L, F, I);
					double beta = DetB / (double) DetA;
					if (beta <= 0)
						return false;
					DetC = Determinant(A, J, G, B, K, H, C, L, I);
					double gamma = DetC / (double) DetA;
					if (gamma <= 0)
						return false;
					if (beta + gamma >= 1)
						return false;
					Point3D TMP = r.Ro + r.Rd * t;
					 
					TMP = Point3D((TMP.x), TMP.y, TMP.z);
					Point3D TMP2 = Point3D((p.x), p.y, p.z);
					TMP = TMP - TMP2;
					//cout << TMP.x << " " << TMP.y << " " << TMP.z << endl;
					//Point3D TMP3 = Point3D(ceil(p.x), ceil(p.y), ceil(p.z));
					//cout << "TMP = " << TMP.x << " " <<  TMP.y << " " << TMP.z << endl;
					
					if(abs(TMP.x) > 1e-11 || abs(TMP.y) > 1e-11 || abs(TMP.z) > 1e-11)
					{
							cout << "p = " << p.x << " " << p.y << " " << p.z << endl;
							return false;
					}
					//else
					//{
					if(polynum == 0)
					{
						result = Vector3D(0,0,-1);
					}
					else
					{						
						Vector3D U(c, b);
						Vector3D V(c, a);
						result = Vector3D(U.j * V.k - U.k * V.j, U.k * V.i - U.i * V.k, U.i * V.j - U.j * V.i);
						result /= result.Norm();					
						Vector3D cdir(centroid, p);
						cdir /= cdir.Norm();
						double angle = acos(result.Dot(cdir));
						//cout <<  angle;
						if (angle < PI / 2)
						{
							result *= -1;
						}
					}					
						return true;
					//}
			//}
	//}
	return false;
}

void RayTrace(const int &height, const int &width, const Scene &s)
{
	time_t timer = time(0);
	double numpixels = height * width * 100;
	//cout << "rendering line number:" << endl;
	pair<Point3D, pair<int, int> > result;
	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			if(castRay( (Ray(Point3D(j,i,0) , Point3D(0,0,1))), s, result))
			{
				
				
				double Ir = 0;
				double Ib = 0;
				double Ig = 0;
				double rColor = 0;
				double gColor = 0;
				double bColor = 0;
				for(int Z = 0; Z < s.lights.size(); ++Z)
				{
					Vector3D result2(0,0,0);
					Point3D p = result.first;
					Vector3D temp(s.lights[Z].loc, p);
					Vector3D L = temp  / temp.Norm();
					if(castShadowRay(result.second.first, result.second.second, p, Ray(s.lights[Z].loc, Point3D(L.i, L.j, L.k)), s, result2))
					{
					 //cout << "Shadow Ray Clear" << endl;
					 double tempdr = L.Dot(result2) * s.lights[Z].krd * s.materials[result.second.first].krd;
					 double tempdg = L.Dot(result2) * s.lights[Z].kgd * s.materials[result.second.first].kgd;
					 double tempdb = L.Dot(result2) * s.lights[Z].kbd * s.materials[result.second.first].kbd;
					 if (tempdr < 0)
						tempdr = 0;
					 if (tempdg < 0)
						tempdg = 0;
					 if (tempdb < 0)
						tempdb = 0;
					 Vector3D V(0,0, 1);
					 V = (V + L);
					 V /= V.Norm();
					 double tempsr = pow(V.Dot(result2), s.materials[result.second.first].alpha) * s.lights[Z].krs * s.materials[result.second.first].krs;
					 double tempsg = pow(V.Dot(result2), s.materials[result.second.first].alpha) * s.lights[Z].kgs * s.materials[result.second.first].kgs;
					 double tempsb = pow(V.Dot(result2), s.materials[result.second.first].alpha) * s.lights[Z].kbs * s.materials[result.second.first].kbs;
					  if (tempsr < 0)
						tempdr = 0;
					 if (tempsg < 0)
						tempdg = 0;
					 if (tempsb < 0)
						tempdb = 0;
					 Ir += s.lights[Z].rColor * s.lights[Z].kra * s.materials[result.second.first].kra + tempdr + tempsr;
					 Ig += s.lights[Z].gColor * s.lights[Z].kga * s.materials[result.second.first].kga + tempdg + tempsg;
					 Ib += s.lights[Z].bColor * s.lights[Z].kba * s.materials[result.second.first].kba + tempdb + tempsb;
					}
					else
					{		
					 //cout << "shadowed" << endl;	        
					 Ir += s.lights[Z].rColor * s.lights[Z].kra * s.materials[result.second.first].kra;
					 Ig += s.lights[Z].gColor * s.lights[Z].kga * s.materials[result.second.first].kga;
					 Ib += s.lights[Z].bColor * s.lights[Z].kba * s.materials[result.second.first].kba;
					}
				}
				if(Ir > 1)
					Ir = 1;
				if(Ig > 1)
					Ig = 1;
				if(Ib > 1)
					Ib = 1;
				//Ib /= s.lights.size();
				//cout << Ir << endl;
				renderPixel(j, i, Ir, Ig, Ib);
			}
			
		}
		cout << "Rendering Line: " << setw(5) << setprecision(2) << showpoint << (i + 1) << " of " << height << " Elapsed:" << time(0) - timer << "s" <<  '\r';
		cout.flush();
	}	
}

#endif


