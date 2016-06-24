#ifndef BIOMEX2_H_
#define BIOMEX2_H_

#include "Includes.h"

class BiomeX2 : public Terrain
{
	public:
		BiomeX2(std::vector<int> liste);
		~BiomeX2();

		virtual int getNombreBiomes();
		virtual std::string getUrl();
};

#endif // BIOMEX2_H_