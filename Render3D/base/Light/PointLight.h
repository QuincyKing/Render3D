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
	};
}