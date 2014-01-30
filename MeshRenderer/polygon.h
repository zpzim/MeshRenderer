#ifndef ___POLY__H_
#define ___POLY__H_

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "point3d.h"
#include "rendering.h"
#include "vectorray.h"

using namespace std;

//This Struct is no longer Nessecary Could be replaced by a Point3D type, but is a lot of work to get rid of.
struct Vertex
{
	Point3D v;

	Vertex(const Vertex &V) : v(V.v) {}
	Vertex(Point3D p) : v(p) {}

};

//A face is a set of 3 indices into a vertex list that defines a connection between those vertices
struct Face
{
	int v1;
	int v2;
	int v3;
	
	Face(const Face &f) : v1(f.v1), v2(f.v2), v3(f.v3) {}
	Face( int p1, int p2, int p3) : v1(p1), v2(p2), v3(p3) {}
	bool operator==(const Face &f) const { return (v1 == f.v1 && v2 == f.v2 && v3 == f.v3); }
	Face& operator=(const Face &rhs) {v1 = rhs.v1; v2 = rhs.v2; v3 = rhs.v3; return *this; }
	
};


//This is the main polygon structure, consisting of a list of points, each with a list of faces they are adjacent to, as well as a list of faces
struct Poly
{

	vector<pair<Vertex, vector< Face > > > verts;
	vector<Face> poly;
	int numVertices;
	int numFaces;	

	Poly() : numVertices(0), numFaces(0), poly(), verts() {}
	Poly(vector<Face> p, vector<pair<Vertex, vector<Face> > >  v) : numVertices(v.size()), numFaces(p.size()), poly(p), verts(v) {}
	Poly(char* infilename);
	void renderfront();
	void rendertop();
	void renderside();
	void addFace(Vertex a, Vertex b, Vertex c);
	void print();
	Poly translate(double x = 0, double y = 0, double z = 0);
	Poly rotatephi(double phi = 0);
	Poly rotatetheta(double theta = 0);
	Poly XYscale(double xamt, double yamt);
	Poly XYZscale(double amt);	
	Poly subdivide();
	Point3D pointAvg() const;
	vector<Face> neighborhood(const Face &f);
	vector<Point3D> neighborhood(const int &p);
	Point3D oddVertexSetup(int a, int b, Face f);
	Point3D evenVertexSetup(int p, const vector<Face> &vf);
};



//Constructs a polygon from a mesh input file
Poly::Poly(char* infilename)
{
	ifstream infile;
	infile.open(infilename);
	if (infile.fail())
	{
		cerr << "Cannot open " << infilename << ", please make sure it is accessable." << endl;
		exit(0);	
	}
	int x,y,z;
	infile >> numVertices;
	infile >> numFaces;
	while (!infile.eof() && verts.size() < numVertices)
	{
		infile >> x;
		infile >> y;
		infile >> z;
		verts.push_back(make_pair(Vertex(Point3D(x * 8 ,y * 8 ,z * 8 )), vector<Face >()));
	}	
	while (!infile.eof() && poly.size() < numFaces)
	{
		infile >> x;
		infile >> y;
		infile >> z;
		Face f = Face( x, y, z);
		poly.push_back(f);
		verts[x].second.push_back((poly[poly.size() - 1]));
		verts[y].second.push_back((poly[poly.size() - 1]));
		verts[z].second.push_back((poly[poly.size() - 1]));
	}
	infile.close();
}

//Finds Neighboring points connected by an edge starting at vertex number p
vector<Point3D> Poly::neighborhood(const int &p)
{
	vector<Point3D> result;	
	vector<Face> temp = verts[p].second;
	//Iterate through the neighboring faces of p, adding vertices on those faces to the result
	//given they are not already in the result and not the point p	
	for(int i = 0; i < temp.size(); ++i)
	{
		if((temp[i].v1 != p) && (find(result.begin(), result.end(), verts[temp[i].v1].first.v) == result.end()))
			result.push_back(verts[temp[i].v1].first.v);
		if((temp[i].v2 != p) && (find(result.begin(), result.end(), verts[temp[i].v2].first.v) == result.end()))
			result.push_back(verts[temp[i].v2].first.v);
		if((temp[i].v3 != p) && (find(result.begin(), result.end(), verts[temp[i].v3].first.v) == result.end()))
			result.push_back(verts[temp[i].v3].first.v);
	}
	return result;
}

//Finds Faces adjacent to Face f
vector<Face> Poly::neighborhood(const Face &f)
{
	vector<Face> result;
	//Iterate through the faces adjacent to v1,v2,v3 of the input parameter, checking if they also share a second point with the input
	//If a 2 faces share 2 vertices, they either share the 3rd vertex and are the same face, OR
	//They share an edge ==> they are an adjacent face. There should only be a total of 3 result inserts in all three loops combined
	//given that faces are triangles, and we are dealing with closed polygons
	for(vector<Face>::iterator it = verts[f.v1].second.begin(); it < verts[f.v1].second.end(); ++it)
	{
		if (*it == f || (find(result.begin(), result.end(), *it) != result.end()))
			continue;	
		if( it -> v1 == f.v2 || it -> v2 == f.v2 || it -> v3 == f.v2)
				result.push_back(*it);
		else if(it -> v1 == f.v3 || it -> v2 == f.v3 || it -> v3 == f.v3)
				result.push_back(*it);	
	
	}
	for(vector<Face>::iterator it = verts[f.v2].second.begin(); it < verts[f.v2].second.end(); ++it)
	{
		if (*it == f || (find(result.begin(), result.end(), *it) != result.end()))
			continue;		
		if( it -> v1 == f.v1 || it -> v2 == f.v1 || it -> v3 == f.v1)
				result.push_back(*it);
		else if(it -> v1 == f.v3 || it -> v2 == f.v3 || it -> v3 == f.v3)
				result.push_back(*it);	
		
	}
	for(vector<Face>::iterator it = verts[f.v3].second.begin(); it < verts[f.v3].second.end(); ++it)
	{	
		if (*it == f || (find(result.begin(), result.end(), *it) != result.end()))
			continue;		
		if( it -> v1 == f.v1 || it -> v2 == f.v1 || it -> v3 == f.v1)
				result.push_back(*it);
		else if(it -> v1 == f.v2 || it -> v2 == f.v2 || it -> v3 == f.v2)
				result.push_back(*it);	
	}
	
	return result;
}

//Sets up an original(even) vertex for smoothing
Point3D Poly::evenVertexSetup(int p, const vector<Face> &vf)
{
	//Find neighboring vertices to p
	vector<Point3D> neighborhood = this -> neighborhood(p);
	Point3D newpt(0,0,0);
	//Add neighbors together weighted by 1/10
	for(int i = 0; i < neighborhood.size(); ++i)
	{
		newpt += neighborhood[i] * (1 / 12.0);
	}
	//Add original p weighted by (1 - n/10) where n is the number of neighbors of p
	newpt += (verts[p].first.v * (1 - (neighborhood.size() * ( 1 / 12.0))));

	return newpt;

}


//Sets up a new(odd) vertex for smoothing
Point3D Poly::oddVertexSetup(int a, int b, Face f)
{
	bool afound = false, bfound = false;
	Point3D result(0,0,0);
	Face temp(0,0,0);
	//Find neigboring faces to f
	vector<Face> nf = neighborhood(f);
	//Iterate through the neighboring faces to find the ONE neighbor that shares both a and b with f
	//This face is the face that shares the new odd vertex with f
	for(int i = 0; i < nf.size(); ++i)
	{
		
		if((nf[i].v1 == a || nf[i].v2 == a || nf[i].v3 == a) && (nf[i].v1 == b || nf[i].v2 == b || nf[i].v3 == b))
		{
			temp = nf[i];
			break;
		}
	}
	//A and B are weighted 3/8 each
	result += verts[a].first.v * 0.375;
	result += verts[b].first.v * 0.375;
	//There is one vertex of the second face that is not equal to a or b, it is weighted 1/8
	if (temp.v1 != a && temp.v1 != b)
		result += verts[temp.v1].first.v * 0.125;
	else if (temp.v2 != a && temp.v2 != b)
		result += verts[temp.v2].first.v * 0.125;
	else if (temp.v3 != a && temp.v3 != b)
		result += verts[temp.v3].first.v * 0.125;
	//There is one vertex of f that is not equal to a or b, it is weighted 1/8
	if (f.v1 != a && f.v1 != b)
		result += verts[f.v1].first.v * 0.125;
	else if (f.v2 != a && f.v2 != b)
		result += verts[f.v2].first.v * 0.125;
	else if (f.v3 != a && f.v3 != b)
		result += verts[f.v3].first.v * 0.125;

	return result;	

}


//Adds the face defined by a,b,c if a, b, or c is not present in polygon it is added as well
void Poly::addFace(Vertex a, Vertex b, Vertex c)
{
	vector<Face> v;
	bool v1Found = false;
	bool v2Found = false;
	bool v3Found = false;
	int idx1 = -1;
	int idx2 = -1;
	int idx3 = -1;
	for(int i = 0; i < verts.size(); ++i)
	{	
		if( !v1Found && verts[i].first.v == a.v)
		{
			idx1 = i;			
			v1Found = true;
		}
		if( !v2Found && verts[i].first.v == b.v)
		{
			idx2 = i;			
			v2Found = true;
		}
		if( !v3Found && verts[i].first.v == c.v)
		{
			idx3 = i;
			v3Found = true;
		}
	}

	Face f(idx1, idx2, idx3);
	if (find(poly.begin(), poly.end(), f) != poly.end())
		return;

	if (!v1Found)
	{
		verts.push_back(make_pair(a, v));
		numVertices++;
		idx1 = verts.size() - 1;
	}		
	if (!v2Found)
	{	
		verts.push_back(make_pair(b, v));
		numVertices++;
		idx2 = verts.size() - 1;
	}
	if (!v3Found)
	{
		verts.push_back(make_pair(c , v));
		numVertices++;
		idx3 = verts.size() - 1;
	}

	poly.push_back(Face(idx1, idx2, idx3));
	verts[idx1].second.push_back((poly[poly.size() - 1]));
	verts[idx2].second.push_back((poly[poly.size() - 1]));
	verts[idx3].second.push_back((poly[poly.size() - 1]));
	numFaces++;
}



// Renders a front view of the polygon
void Poly::renderfront()
{
	//Front View is a projection onto the XY-plane
	for (int i = 0; i < poly.size(); ++i)
	{
		//cerr << "Rendering Front";
		renderLine(verts[poly[i].v1].first.v.x, verts[poly[i].v1].first.v.y, verts[poly[i].v2].first.v.x, verts[poly[i].v2].first.v.y);
		renderLine(verts[poly[i].v1].first.v.x, verts[poly[i].v1].first.v.y, verts[poly[i].v3].first.v.x, verts[poly[i].v3].first.v.y);
		renderLine(verts[poly[i].v2].first.v.x, verts[poly[i].v2].first.v.y, verts[poly[i].v3].first.v.x, verts[poly[i].v3].first.v.y);
	}
}

// Renders a top view of the polygon
void Poly::rendertop()
{
	//Top View is a projection onto the XZ-plane
	for (int i = 0; i < poly.size(); ++i)
	{
		renderLine(verts[poly[i].v1].first.v.x, verts[poly[i].v1].first.v.z, verts[poly[i].v2].first.v.x, verts[poly[i].v2].first.v.z);
		renderLine(verts[poly[i].v1].first.v.x, verts[poly[i].v1].first.v.z, verts[poly[i].v3].first.v.x, verts[poly[i].v3].first.v.z);
		renderLine(verts[poly[i].v2].first.v.x, verts[poly[i].v2].first.v.z, verts[poly[i].v3].first.v.x, verts[poly[i].v3].first.v.z);
	}

}

// Renders a side view of the polygon
void Poly::renderside()
{
	//
	for (int i = 0; i < poly.size(); ++i)
	{
		renderLine(verts[poly[i].v1].first.v.z, verts[poly[i].v1].first.v.y, verts[poly[i].v2].first.v.z, verts[poly[i].v2].first.v.y);
		renderLine(verts[poly[i].v1].first.v.z, verts[poly[i].v1].first.v.y, verts[poly[i].v3].first.v.z, verts[poly[i].v3].first.v.y);
		renderLine(verts[poly[i].v2].first.v.z, verts[poly[i].v2].first.v.y, verts[poly[i].v3].first.v.z, verts[poly[i].v3].first.v.y);
	}
}

// Prints polygon info (Debugging purposes)
void Poly::print()
{
	for(int i = 0; i < poly.size(); ++i)
	{
		cerr << "Face " << i << endl;
		cerr << "v1: x = " << verts[poly[i].v1].first.v.x << " y = " << verts[poly[i].v1].first.v.y << " z = " << verts[poly[i].v1].first.v.z;
		cerr << "v2: x = " << verts[poly[i].v2].first.v.x << " y = " << verts[poly[i].v2].first.v.y << " z = " << verts[poly[i].v2].first.v.z;
		cerr << "v3: x = " << verts[poly[i].v3].first.v.x << " y = " << verts[poly[i].v3].first.v.y << " z = " << verts[poly[i].v3].first.v.z;
	}


}

Poly Poly::XYscale(double xamt, double yamt)
{
	Poly newPoly(poly, verts);
	for (int i = 0; i < verts.size(); ++i)
	{
		newPoly.verts[i].first.v.x *= xamt;
		newPoly.verts[i].first.v.y *= yamt;	
	}
	return newPoly;

}	

Poly Poly::XYZscale(double amt)
{
	Poly newPoly(poly, verts);
	for (int i = 0; i < verts.size(); ++i)
	{
		newPoly.verts[i].first.v.x *= amt;
		newPoly.verts[i].first.v.y *= amt;
		newPoly.verts[i].first.v.z *= amt;	
	}
	return newPoly;


}

Poly Poly::translate(double x, double y, double z)
{
	Poly newPoly(poly, verts);
	for (int i = 0; i < verts.size(); ++i)
	{
		newPoly.verts[i].first.v.x += x;
		newPoly.verts[i].first.v.y += y;
		newPoly.verts[i].first.v.z += z;  	
	}
	return newPoly;
}

Poly Poly::rotatephi(double phi)
{
	if (phi == 0)
		return *this;
	Poly newPoly(poly, verts);
	Point3D centroid(newPoly.pointAvg());	
	newPoly = newPoly.translate( -centroid.x, -centroid.y, -centroid.z);
	Poly copy(newPoly.poly, newPoly.verts);
	double a = sin(phi);
	double b = cos(phi);
	for (int i = 0; i < newPoly.verts.size(); ++i)
	{
		newPoly.verts[i].first.v.x = copy.verts[i].first.v.x * b + copy.verts[i].first.v.z * a;
		newPoly.verts[i].first.v.z = -copy.verts[i].first.v.x * a + copy.verts[i].first.v.z * b;
	}
	newPoly = newPoly.translate( centroid.x, centroid.y, centroid.z);
	return newPoly;
}

Poly Poly::rotatetheta(double theta)
{
	if (theta == 0)
		return *this;
	Poly newPoly(poly, verts);
	Point3D centroid(newPoly.pointAvg());	
	newPoly = newPoly.translate( -centroid.x, -centroid.y, -centroid.z);
	Poly copy(newPoly.poly, newPoly.verts);
	double a = sin(theta);
	double b = cos(theta);
	for (int i = 0; i < newPoly.verts.size(); ++i)
	{
		newPoly.verts[i].first.v.y = copy.verts[i].first.v.y * b - copy.verts[i].first.v.z * a;
		newPoly.verts[i].first.v.z = copy.verts[i].first.v.y * a + copy.verts[i].first.v.z * b;
	}
	newPoly = newPoly.translate( centroid.x, centroid.y, centroid.z);
	return newPoly;

}




// Subdivdes and smooths polygon
Poly Poly::subdivide()
{
	Point3D temp(0,0,0);
	Poly newPoly;
	for( int i = 0; i < poly.size(); ++i)
	{	
		//
		Vertex nv1(oddVertexSetup(poly[i].v1, poly[i].v2, poly[i]));
		Vertex nv2(oddVertexSetup(poly[i].v1, poly[i].v3, poly[i]));
		Vertex nv3(oddVertexSetup(poly[i].v2, poly[i].v3, poly[i]));
		newPoly.addFace(nv1, nv2, nv3);
		nv1 = Vertex(evenVertexSetup(poly[i].v1, verts[poly[i].v1].second));
		nv2 = Vertex(oddVertexSetup(poly[i].v1, poly[i].v2, poly[i]));
		nv3 = Vertex(oddVertexSetup(poly[i].v1, poly[i].v3, poly[i]));
		newPoly.addFace(nv1, nv2, nv3);
		nv1 = Vertex(evenVertexSetup(poly[i].v2, verts[poly[i].v2].second));
		nv2 = Vertex(oddVertexSetup(poly[i].v1, poly[i].v2, poly[i]));
		nv3 = Vertex(oddVertexSetup(poly[i].v2, poly[i].v3, poly[i]));
		newPoly.addFace(nv1, nv2, nv3);
		nv1 = Vertex(evenVertexSetup(poly[i].v3, verts[poly[i].v3].second));
		nv2 = Vertex(oddVertexSetup(poly[i].v2, poly[i].v3, poly[i]));
		nv3 = Vertex(oddVertexSetup(poly[i].v1, poly[i].v3, poly[i]));
		newPoly.addFace(nv1, nv2, nv3);
	}
	return newPoly;
	
}

Point3D Poly::pointAvg() const
{
	
	double resultA = 0;
	Point3D resultAavg(0.0,0.0,0.0);
	for (int i = 0; i < poly.size(); ++i)
	{
		Point3D a = verts[poly[i].v1].first.v;
		Point3D b = verts[poly[i].v2].first.v;
		Point3D c = verts[poly[i].v3].first.v;
		Vector3D U(c, a);
		Vector3D V(c, b);
		Vector3D r = Vector3D(U.j * V.k - U.k * V.j, U.k * V.i - U.i * V.k, U.i * V.j - U.j * V.i);
		double A = r.Norm();
		Point3D avg = (a + b + c) / 3.0;
		resultAavg += avg * A;
		resultA += A;
	}
	
	return resultAavg / resultA;

}


#endif
