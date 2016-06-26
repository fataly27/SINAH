#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <string>

#include "Constants.h"

class Terrain
{
	public:
		virtual int getNombreBiomes() = 0;
		virtual std::string getUrl() = 0;

	protected:
		std::string mUrl;
};

#endif // TERRAIN_H_