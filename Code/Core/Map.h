#ifndef MAP_H_
#define MAP_H_

#include <array>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <memory>

#include "Constants.h"
#include "BiomeX1.h"
#include "BiomeX2.h"
#include "BiomeX3.h"

class Map
{
	public:
		Map();
		~Map();
		bool generateProceduralMap();

		std::array<std::array<std::shared_ptr<Terrain>, 5>, 5> getMap();

	private:
		std::vector<int> generateOneTerrain(int nbBiomes, std::array<int, 4>& nbTypeBiome);

		std::mt19937 mEngine;
		std::array<std::array<std::shared_ptr<Terrain>, 5>, 5> mMap;
};

#endif // MAP_H_