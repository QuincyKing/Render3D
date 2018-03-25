#pragma once

namespace Base3D
{
     class Color
	 {
	 public:
		 float r, g, b, a;

		 Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) { }
		 Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) { }
		 Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { }
		 Color operator *(float scale) const;
		 Color& operator =(Color &_color);
		 Color operator +(Color _color) const;
		 Color& operator +=(Color _color);
		 Color operator *(Color _color) const;
		 Color operator *(float _color[3]) const;
		 Color operator -(Color _color) const;
	 };

	 static void Interpolating(Color &res, const Color &src1, const Color &src2, const Color &src3
		 , float a, float b, float c)
	 {
		 Color tmp;
		 tmp = src1 * a;
		 tmp += src2 * b;
		 tmp += src3 * c;
		 res = tmp;
	 }
}