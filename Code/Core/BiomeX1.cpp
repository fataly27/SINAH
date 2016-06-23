#include "BiomeX1.h"

BiomeX1::BiomeX1(std::vector<int> liste)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 engine(seed);
	std::uniform_int_distribution<int> distribution(1, 3);
	int version = distribution(engine);

	mUrl = "Landscapes/HeightMaps/1Biome/" + std::to_string(liste[0]) + "/" + std::to_string(version) + ".png";
}
BiomeX1::~BiomeX1() {}

int BiomeX1::getNombreBiomes()
{
	return 1;
}
std::string BiomeX1::getUrl()
{
	return mUrl;
}