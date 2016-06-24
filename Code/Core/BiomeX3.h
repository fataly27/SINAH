#ifndef BIOMEX3_H_
#define BIOMEX3_H_

#include "Includes.h"

class BiomeX3 : public Terrain
{
	public:
		BiomeX3(std::vector<int> liste);
		~BiomeX3();

		virtual int getNombreBiomes();
		virtual std::string getUrl();
};

#endif // BIOMEX3_H_