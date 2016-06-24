#ifndef BIOMEX1_H_
#define BIOMEX1_H_

#include "Includes.h"

class BiomeX1 : public Terrain
{
	public:
		BiomeX1(std::vector<int> liste);
		~BiomeX1();

		virtual int getNombreBiomes();
		virtual std::string getUrl();
};

#endif // BIOMEX1_H_