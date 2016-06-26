#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <array>

namespace CameraConstants
{
	const int ticksOffset = 4000;
	const int minTicks = 0;
	const int maxTicks = 8000;
	const float speed = 1.5;
	const float sensisivity = 700;
	const int nbFramesTransition = 16;
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
