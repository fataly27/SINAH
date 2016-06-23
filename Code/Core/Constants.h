#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <array>

namespace CameraConstants
{
	const int ticksOffset = 1100;
	const int minTicks = 0;
	const int maxTicks = 1000;
	const float speed = 0.3;
	const float sensisivity = 60;
	const int nbFramesTransition = 15;
}
namespace GeneralConstants
{
	const int nbFrames = 60;
}
namespace TerrainConstants
{
	const std::array<int, 4> nbTypeBiomeMax = { 12, 12, 12, 12 };
	const std::array<int, 3> nbTypeTerrain = { 18, 4, 3 };
}

#endif // CONSTANTS_H_
