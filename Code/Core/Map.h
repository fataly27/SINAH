#ifndef MAP_H_
#define MAP_H_

#include "Includes.h"

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