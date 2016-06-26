#ifndef BIOMEX2_H_
#define BIOMEX2_H_

#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>

#include "Terrain.h"

#include "Constants.h"

class BiomeX2 : public Terrain
{
	public:
		BiomeX2(std::vector<int> liste);
		~BiomeX2();

		virtual int getNombreBiomes();
		virtual std::string getUrl();
};

#endif // BIOMEX2_H_