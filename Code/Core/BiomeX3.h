#ifndef BIOMEX3_H_
#define BIOMEX3_H_

#include <array>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <memory>
#include <string>

#include "Terrain.h"

class BiomeX3 : public Terrain
{
	public:
		BiomeX3(std::vector<int> liste);
		~BiomeX3();

		virtual int getNombreBiomes();
		virtual std::string getUrl();
};

#endif // BIOMEX3_H_