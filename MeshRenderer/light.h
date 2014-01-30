#ifndef _LIG_H_T__H
#define _LIG_H_T__H

#include "point3d.h"
#include<vector>
using namespace std;

struct Light
{
	Light(const Point3D &a, const vector<double> &nr, const vector<double> &ng, const vector<double> &nb) : loc(a), rColor(nr[0]), kra(nr[1]), krd(nr[2]), krs(nr[3]), gColor(ng[0]), kga(ng[1]), kgd(ng[2]), kgs(ng[3]), bColor(nb[0]), kba(nb[1]), kbd(nb[2]), kbs(nb[3]) {}
	Point3D loc;
	double rColor;
	double gColor;
	double bColor;
	double kra; double krd; double krs;
	double kga; double kgd; double kgs;
	double kba; double kbd; double kbs;
};

struct Material
{
	
	Material(const vector<double> &nr, const vector<double> &ng, const vector<double> &nb, const double &a) : kra(nr[0]), krd(nr[1]), krs(nr[2]), kga(ng[0]), kgd(ng[1]), kgs(ng[2]), kba(nb[0]), kbd(nb[1]), kbs(nb[2]), alpha(a) {}
	Material() : kra(.2), krd(.2), krs(.2), kga(.2), kgd(.2), kgs(.2), kba(.2), kbd(.2), kbs(.2), alpha(50) {}
	double kra, krd, krs;
	double kga, kgd, kgs;
	double kba, kbd, kbs;
	double alpha;
};

#endif
