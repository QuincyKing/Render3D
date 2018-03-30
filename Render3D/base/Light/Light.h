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

		Light() = default;

		Light(Color abmi, Color diff, Color spe, bool sha) : abmient(abmi), diffuse(diff), specular(spe), shadow(sha)
		{

		}
	};
}