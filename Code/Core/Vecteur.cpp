#include "Includes.h"

Vecteur::Vecteur() : Vecteur(0.0f, 0.0f, 0.0f) {}
Vecteur::Vecteur(float x, float y, float z) : x(x), y(y), z(z) {}
Vecteur::~Vecteur() {}

float Vecteur::absoluteValue(float value)
{
	if (value < 0.0)
		return -value;
	else
		return value;
}
int Vecteur::absoluteValue(int value)
{
	if (value < 0)
		return -value;
	else
		return value;
}
double Vecteur::absoluteValue(double value)
{
	if (value < 0)
		return -value;
	else
		return value;
}


bool Vecteur::testEgalite(Vecteur* autre)
{
	return (this->x == autre->x && this->y == autre->y && this->z == autre->z);
}
void Vecteur::operator+= (Vecteur autre)
{
	this->x += autre.x;
	this->y += autre.y;
	this->z += autre.z;
}
void Vecteur::operator-= (Vecteur autre)
{
	this->x -= autre.x;
	this->y -= autre.y;
	this->z -= autre.z;
}
void Vecteur::operator*= (Vecteur autre)
{
	float x = this->y * autre.z - this->z * autre.y;
	float y = this->x * autre.z - this->z * autre.x;
	float z = this->x * autre.y - this->y * autre.x;

	this->x = x;
	this->y = y;
	this->z = z;
}
void Vecteur::operator*= (float nombre)
{
	this->x *= nombre;
	this->y *= nombre;
	this->z *= nombre;
}
void Vecteur::operator*= (double nombre)
{
	this->x *= nombre;
	this->y *= nombre;
	this->z *= nombre;
}
void Vecteur::operator*= (int nombre)
{
	this->x *= nombre;
	this->y *= nombre;
	this->z *= nombre;
}
float Vecteur::division (Vecteur autre)
{
	if (autre.x != 0.0)
		return this->x / autre.x;
	else if (autre.y != 0.0)
		return this->y / autre.y;
	else if (autre.z != 0.0)
		return this->z / autre.z;
	else
		return 0;
}
void Vecteur::operator/= (float nombre)
{
	this->x /= nombre;
	this->y /= nombre;
	this->z /= nombre;
}
void Vecteur::operator/= (int nombre)
{
	this->x /= nombre;
	this->y /= nombre;
	this->z /= nombre;
}
void Vecteur::operator/= (double nombre)
{
	this->x /= nombre;
	this->y /= nombre;
	this->z /= nombre;
}
void Vecteur::afficher(std::ostream &flux) const
{
	flux << "(" << this->x << "; " << this->y << "; " << this->z << ")";
}

bool operator== (Vecteur a, Vecteur b)
{
	return a.testEgalite(&b);
}
bool operator!= (Vecteur a, Vecteur b)
{
	return !(a == b);
}
Vecteur operator+ (Vecteur a, Vecteur b)
{
	a += b;
	return a;
}
Vecteur operator- (Vecteur a, Vecteur b)
{
	a -= b;
	return a;
}
Vecteur operator* (Vecteur a, Vecteur b)
{
	a *= b;
	return a;
}
Vecteur operator* (Vecteur a, float nombre)
{
	a *= nombre;
	return a;
}
Vecteur operator* (Vecteur a, int nombre)
{
	a *= nombre;
	return a;
}
Vecteur operator* (Vecteur a, double nombre)
{
	a *= nombre;
	return a;
}
float operator/ (Vecteur a, Vecteur b)
{
	return a.division(b);
}
Vecteur operator/ (Vecteur a, float nombre)
{
	a /= nombre;
	return a;
}
Vecteur operator/ (Vecteur a, int nombre)
{
	a /= nombre;
	return a;
}
Vecteur operator/ (Vecteur a, double nombre)
{
	a /= nombre;
	return a;
}

std::ostream &operator<<(std::ostream &flux, Vecteur const& Vecteur)
{
	Vecteur.afficher(flux);
	return flux;
}
