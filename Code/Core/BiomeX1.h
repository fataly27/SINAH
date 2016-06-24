#ifndef BIOMEX1_H_
#define BIOMEX1_H_

#include <array>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <memory>
#include <string>

#include "Terrain.h"

class BiomeX1 : public Terrain
{
	public:
		BiomeX1(std::vector<int> liste);
		~BiomeX1();

		virtual int getNombreBiomes();
		virtual std::string getUrl();
};

#endif // BIOMEX1_H_