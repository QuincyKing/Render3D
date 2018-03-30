#pragma once

#include "Light.h"
#include "../../math/Vector.h"

namespace Base3D
{
	class DirLight : public Light
	{
	public:
		Math3D::Vector4 direction;

		DirLight() = default;
		DirLight(Math3D::Vector4 dir, Color abmi, Color diff, Color spe, bool sha)
			:Light(abmi, diff, spe, sha), direction(dir)
		{

		}
	};
}