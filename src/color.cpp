#include "color.h"
#include <assert.h>

//Default Konstruktor, initialisiert die Farbe mit schwarz
Color::Color()
{
	this->R = 0.0f;
	this->G = 0.0f;
	this->B = 0.0f;
}

//Konstruktor, welches die einzelnen Farbkanäle mit den übergebenen Parametern initialisiert
Color::Color(float r, float g, float b)
{
	this->R = r;
	this->G = g;
	this->B = b;
}

//this und die übergebene Farbe multiplizieren ihre Farbkanäle, Rückgabe ist ne neue Farbe
Color Color::operator*(const Color& c) const
{
	return Color(this->R*c.R, this->G*c.G, this->B*c.B);
}

//Die Farbkanäle der Farbe werden mit einem als Parameter übergebenen Faktor multipliziert, Rückgabe ist ne neue Farbe
Color Color::operator*(const float Factor) const
{
	return Color(this->R * Factor, this->G * Factor, this->B * Factor);
}

//this und die übergebene Farbe addieren ihre Farbkanäle, Rückgabe ist ne neue Farbe
Color Color::operator+(const Color& c) const
{
	return Color(this->R + c.R, this->G + c.G, this->B + c.B);
}

//this wird mit der übergebenen Farbe verändert, Rückgabe ist this
Color& Color::operator+=(const Color& c)
{
	this->R += c.R;
	this->G += c.G;
	this->B += c.B;

	return *this;
}