#pragma once

namespace Base3D
{
	class Storage
	{
	public:
		float a, b, c, d;

		Storage operator +(Storage stor)
		{
			Storage tmp;
			tmp.a = this->a + stor.a;
			tmp.b = this->b + stor.b;
			tmp.c = this->c + stor.c;
			tmp.d = this->d + stor.d;

			return tmp;
		}

		Storage operator *(float _scale)
		{
			Storage tmp;
			tmp.a = this->a * _scale;
			tmp.b = this->b * _scale;
			tmp.c = this->c * _scale;
			tmp.d = this->d * _scale;

			return tmp;
		}
	};
}