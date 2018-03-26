#pragma once
#include "../render/Primitive.h"
#include "../math/Vector.h"
#include "../math//Matrix.h"

namespace Base3D
{
	class Object
	{
	public:
		Render3D::Vertex *mesh;
		unsigned long mesh_num;
		int* material_ids;
		int texture_id;
		bool shadow;

		bool dirty;
		Render3D::Point pos;
		Math3D::Vector4 scale;

		Math3D::Vector4 axis;
		float theta;

		Math3D::Matrix44 matrix;
	};
}