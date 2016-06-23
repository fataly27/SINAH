#ifndef VECTEUR_H_
#define VECTEUR_H_

#include <iostream>

class Vecteur
{
	public:
		Vecteur();
		Vecteur(float x, float y, float z);
		~Vecteur();

		float x;
		float y;
		float z;

		bool testEgalite(Vecteur* autre);
		void operator+= (Vecteur autre);
		void operator-= (Vecteur autre);
		void operator*= (Vecteur autre);
		void operator*= (float nombre);
		void operator*= (int nombre);
		void operator*= (double nombre);
		float division (Vecteur autre);
		void operator/= (float nombre);
		void operator/= (int nombre);
		void operator/= (double nombre);
		void afficher(std::ostream &flux) const;

		static float absoluteValue(float value);
		static int absoluteValue(int value);
		static double absoluteValue(double value);
};

bool operator== (Vecteur a, Vecteur b);
bool operator!= (Vecteur a, Vecteur b);

Vecteur operator+ (Vecteur a, Vecteur b);
Vecteur operator- (Vecteur a, Vecteur b);
Vecteur operator* (Vecteur a, Vecteur b);
Vecteur operator* (Vecteur a, float nombre);
Vecteur operator* (Vecteur a, int nombre);
Vecteur operator* (Vecteur a, double nombre);
float operator/ (Vecteur a, Vecteur b);
Vecteur operator/ (Vecteur a, float nombre);
Vecteur operator/ (Vecteur a, int nombre);
Vecteur operator/ (Vecteur a, double nombre);

std::ostream &operator<<(std::ostream &flux, Vecteur const& Vecteur);

#endif // VECTEUR_H_
