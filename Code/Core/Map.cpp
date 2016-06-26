#include "Map.h"

Map::Map() : mEngine(std::chrono::system_clock::now().time_since_epoch().count()) {}
Map::~Map() {}

bool Map::generateProceduralMap()
{
	std::array<int, 4> nbTypeBiome = { 0, 0, 0, 0 };
	std::array<std::vector<std::vector<int>>, 3> biomes;
	int i = 0;

	for (std::vector<std::vector<int>>& biomes2 : biomes)
	{
		for (int j = 0; j < TerrainConstants::nbTypeTerrain[i]; j++)
		{
			biomes2.push_back(generateOneTerrain(i + 1, nbTypeBiome));

			if (biomes2[j].empty())
				return false;
		}

		std::shuffle(biomes[i].begin(), biomes[i].end(), mEngine);
		i++;
	}

	i = 0;
	int j = 0;
	for (std::array<std::shared_ptr<Terrain>, 5>& tableauBiomes : mMap)
	{
		j = 0;
		for (std::shared_ptr<Terrain>& biome : tableauBiomes)
		{
			if (i == 0 && j == 0 || i == 2 && j == 2 || i == 4 && j == 4)
			{
				biome.reset(new BiomeX3(biomes[2].back()));
				biomes[2].pop_back();
			}
			else if (i == 2 && j == 0 || i == 0 && j == 2 || i == 2 && j == 4 || i == 4 && j == 2)
			{
				biome.reset(new BiomeX2(biomes[1].back()));
				biomes[1].pop_back();
			}	
			else
			{
				biome.reset(new BiomeX1(biomes[0].back()));
				biomes[0].pop_back();
			}
			j++;
		}
		i++;
	}

	return true;
}
std::vector<int> Map::generateOneTerrain(int nbBiomes, std::array<int, 4>& nbTypeBiome)
{
	std::vector<int> possibilites = { 1, 2, 3, 4 };
	std::shuffle(possibilites.begin(), possibilites.end(), mEngine);
	std::vector<int> resultats;
	int i = nbBiomes;

	while (i != 0)
	{
		int choix = possibilites.back();
		possibilites.pop_back();

		if (nbTypeBiome[choix - 1] < TerrainConstants::nbTypeBiomeMax[choix - 1])
		{
			resultats.push_back(choix);
			nbTypeBiome[choix - 1]++;
			i--;
		}
		else if (possibilites.empty())
			return possibilites;
	}

	return resultats;
}

std::array<std::array<std::shared_ptr<Terrain>, 5>, 5> Map::getMap()
{
	return mMap;
}