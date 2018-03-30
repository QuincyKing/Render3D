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
}
