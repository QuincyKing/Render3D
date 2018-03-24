/*!
* @file Primitive.h
* @date 2017/05/13 16:47
*
* @author QuincyKing
* Contact: j88172365@gmail.com
*
* @brief 图元装配
*/
#pragma  once

#include <cassert>
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "../math/MathUtil.h"
#include "../base/Color.h"

using namespace Math3D;
using namespace Base3D;

namespace Render3D
{
	using Point = Vector4;
	using Texcoord = struct { float u, v; };

	struct Vertex
	{
		Point pos;
		Texcoord tc;
		Color color;
		float rhw;

		void InitWithRhw();
		void Interp(const Vertex x1, const Vertex x2, float t);
		void Division(const Vertex x1, const Vertex x2, float w);
		void Add(const Vertex x);
	};

	struct Edge
	{
		Vertex v, v1, v2;
	};

	struct Scanline
	{
		Vertex v, step; //扫描线的左边点的坐标和步长
		int x, y;		//扫描线起点像素
		int w;			//扫描线的宽度
	};

	struct Trapezoid
	{
		float top, bottom;
		Edge left, right;

		void InterpEdge(float y);
		void InitScanline(Scanline &scanline, int y);
	};

	struct Trapezoids
	{
		Trapezoid trap[2];
		int count;

		Trapezoid& operator[](const int k);
		void InitTriangle(const Vertex p1, const Vertex p2, const Vertex p3);
	};
}