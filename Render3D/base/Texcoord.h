#pragma once 

namespace Base3D
{
	class Texcoord
	{
	public:
		float u, v;

		Texcoord() = default;
		Texcoord(float _u, float _v) :u(_u), v(_v) {}
		Texcoord operator+(Texcoord a)
		{
			Texcoord tmp;
			tmp.u = this->u + a.u;
			tmp.v = this->v + a.v;

			return tmp;
		}

		Texcoord operator *(float _scale)
		{
			Texcoord tmp;
			tmp.u = this->u * _scale;
			tmp.v = this->v * _scale;

			return tmp;
		}
	};

	static void interpolating(Texcoord &dest, const Texcoord &src1, const Texcoord &src2, const Texcoord &src3, float a, float b, float c)
	{
		dest.u = dest.v = 0.0f;
		Texcoord each = src1;
		each = each * a;
		dest = dest + each;
		each = src2;
		each = each * b;
		dest = dest + each;
		each = src3;
		each = each * c;
		dest = dest + each;
	}
}