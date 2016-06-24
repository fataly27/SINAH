#include "BiomeX3.h"

BiomeX3::BiomeX3(std::vector<int> liste)
{
	std::sort(liste.begin(), liste.end());

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 engine(seed);
	std::uniform_int_distribution<int> distribution(1, 2);
	int version = distribution(engine);
	
	mUrl = "Landscapes/HeightMaps/1Biome/" + std::to_string(liste[0]) + "-" + std::to_string(liste[1]) + "-" + std::to_string(liste[2]) + "/" + std::to_string(version) + ".png";
}
BiomeX3::~BiomeX3() {}

int BiomeX3::getNombreBiomes()
{
	return 1;
}
std::string BiomeX3::getUrl()
{
	return mUrl;
}