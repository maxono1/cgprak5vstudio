#include "vector.h"
#include <assert.h>
#include <math.h>

#define EPSILON 1e-6
//Konstruktor, initialisiert die Werte
Vector::Vector(float x, float y, float z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}
//Default Konstruktor, initialisiert alles mit 0
Vector::Vector()
{
	this->X = 0.0f;
	this->Y = 0.0f;
	this->Z = 0.0f;
}
//Skalarprodukt, Rückgabe float
//Formel: a->  *  b->  = x1*x2+y1*y2+z1*z2
float Vector::dot(const Vector &v) const
{
	return ((this->X * v.X) + (this->Y * v.Y) + (this->Z * v.Z));
}
//Kreuzprodukt, Rückgabe Vektor
//Formel: a-> x b-> = c-> mit x=(y1*z2)-(z1*y2), y=(z1*x2)-(x1*z2) und z=(x1*y2)-(y1*x2)
Vector Vector::cross(const Vector &v) const
{
	return Vector((this->Y * v.Z) - (this->Z * v.Y), (this->Z * v.X) - (this->X * v.Z), (this->X * v.Y) - (this->Y * v.X));
}

//Die Vektoren werden normal Komponentenweise addiert
Vector Vector::operator+(const Vector &v) const
{
	return Vector(this->X + v.X, this->Y + v.Y, this->Z + v.Z);
}

//Die Vektoren werden normal Komponentenweise subtrahiert
Vector Vector::operator-(const Vector &v) const
{
	return Vector(this->X - v.X, this->Y - v.Y, this->Z - v.Z);
}
//ein Vektor wird mit einem Faktor multipliziert
Vector Vector::operator*(float c) const
{
	return Vector(this->X * c, this->Y * c, this->Z * c);
}
//alle Werte des Vektors werden mit *-1 multipliziert
Vector Vector::operator-() const
{
	return Vector(-1 * this->X, -1 * this->Y, -1 * this->Z);
}
//Der Vektor wird mit einem anderen Vektor addiert, die Werte werden übernommen von this
Vector &Vector::operator+=(const Vector &v)
{
	this->X += v.X;
	this->Y += v.Y;
	this->Z += v.Z;
	return *this;
}

//ein Vektor wird normalisiert
Vector &Vector::normalize()
{
	float length = this->length();
	if (!(length == 0))
	{
		this->X /= length;
		this->Y /= length;
		this->Z /= length;
	}
	return *this;
}

//das gibt die Länge des Vektors zurück
float Vector::length() const
{
	return sqrtf(this->lengthSquared());
}

//dies ist die berechnung unter dem Wurzelzeichen
float Vector::lengthSquared() const
{
	return (this->X*this->X) + (this->Y*this->Y) + (this->Z*this->Z);
}

// Formel: d - 2 * (d * n) n
Vector Vector::reflection(const Vector &normal) const
{
	Vector vector = *this + (normal * (this->dot(normal)) * (-2));
	return vector;
}

bool Vector::triangleIntersection(const Vector &d, const Vector &a, const Vector &b, const Vector &c, float &s) const
{
	// n berechnen
	Vector n = (b - a).cross(c - a).normalize();
	// d ausrechnen
	float direction = n.dot(a);
	// s ausrechnen
	s = (direction - (n.dot(*this))) / (n.dot(d));

	if (s<0)
	{
		s *= -1;
	}

	// Strahlgleichung
	Vector p = *this + (d * s);
	// Großes Dreieck
	float flaecheDreieck = (((b - a).cross(c - a)).length()) / 2;
	//kleine Dreiecke Fläche ausrechnen
	float dreieck1 = (((b - a).cross(p - a)).length()) / 2;
	float dreieck2 = (((c - b).cross(p - b)).length()) / 2;
	float dreieck3 = (((a - c).cross(p - c).length())) / 2;
	//true wenn Fläche großDreieck >= addition Fläche kleinDreiecke sonst false
	if (flaecheDreieck + EPSILON >= (dreieck1 + dreieck2 + dreieck3)) {
		return true;
	}
	else
	{
		return false;
	}
}

Vector Vector::normal(const Vector& a, const Vector& b, const Vector& c)
{
	return (b-a).cross(c-a);
}
