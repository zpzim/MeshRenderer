#ifndef _V_RAY_H
#define _V_RAY_H

#include<cmath>

struct Vector3D
{
	double i, j, k;
	Vector3D(double a, double b , double c) : i(a), j(b), k(c) {}
	Vector3D(Point3D a, Point3D b) : i(b.x - a.x) , j(b.y - a.y), k(b.z - a.z) {}
	Vector3D(Vector3D a, Vector3D b) : i(b.i - a.i), j(b.j - a.j), k(b.k - a.k) {}
	double Dot(Vector3D &rhs) {return i * rhs.i + j * rhs.j + k * rhs.k; }
	double Norm() {return sqrt( this -> Dot(*this) ); }
	Vector3D operator+(const Vector3D & rhs) const { return Vector3D(i + rhs.i, j + rhs.j, k + rhs.k); }
	Vector3D operator-(const Vector3D & rhs) const { return Vector3D(i - rhs.i, j - rhs.j, k - rhs.k); }
	Vector3D operator*(double val) const { return Vector3D(i * val, j * val, k * val); }
	Vector3D operator/(double val) const { return Vector3D(i / val, j / val, k / val); }
	Vector3D operator+=(const Vector3D & rhs) { i += rhs.i; j += rhs.j; k += rhs.k; return *this; }
	Vector3D operator-=(const Vector3D & rhs) { i -= rhs.i; j -= rhs.j; k -= rhs.k; return *this; }
	Vector3D operator*=(double val) { i *= val; j *= val; k *= val; return *this; }
	Vector3D operator/=(double val) { i /= val; j /= val; k /= val; return *this; }
};

struct Ray
{
	Point3D Ro;
	Point3D Rd;
	Ray(const Point3D &o, const Point3D &d) : Ro(o), Rd(d) {}
};

#endif
