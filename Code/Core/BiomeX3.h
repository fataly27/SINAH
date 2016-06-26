#ifndef BIOMEX3_H_
#define BIOMEX3_H_

#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>

#include "Terrain.h"

#include "Constants.h"

class BiomeX3 : public Terrain
{
	public:
		BiomeX3(std::vector<int> liste);
		~BiomeX3();

		virtual int getNombreBiomes();
		virtual std::string getUrl();
};

#endif // BIOMEX3_H_