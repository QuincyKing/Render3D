#pragma once

#include "Light.h"
#include "../../math/Vector.h"

namespace Base3D
{
	class PointLight : public Light
	{
	public:
		Math3D::Vector4 position;
		float constant;
		float linear;
		float quadratic;

		PointLight() = default;
		PointLight(Math3D::Vector4 pos, float constant, float linear, float qu, Color abmi, Color diff, Color spe, bool sha)
			:Light(abmi, diff, spe, sha), position(pos), constant(constant), linear(linear), quadratic(qu)
		{

		}
	};
}