#pragma once

#include "Light.h"
#include "../../math/Vector.h"

namespace Base3D
{
	class DirLight : public Light
	{
	public:
		Math3D::Vector4 direction;
	};
}