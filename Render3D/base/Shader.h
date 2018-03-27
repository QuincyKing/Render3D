#pragma once

#include "../math/Vector.h"
#include "Color.h"
#include "Texcoord.h"

namespace Base3D
{
	struct a2v
	{
		Math3D::Vector4 pos;
		Color color;
		Math3D::Vector4 normal;
		Math3D::Vector4 tangent;
		Math3D::Vector4 binormal;
		Texcoord texcoord;
	};

	struct v2f
	{
		Math3D::Vector4 pos;
		Texcoord texcoord;
		Color color;
		Math3D::Vector4 normal;
		Math3D::Vector4 storage0;
		Math3D::Vector4 storage1;
		Math3D::Vector4 storage2;
	};
}
