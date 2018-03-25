#pragma once

#include "../Color.h"

namespace Base3D
{
	class Light
	{
	public:
		Color abmient;
		Color diffuse;
		Color specular;
		bool shadow;
	};
}