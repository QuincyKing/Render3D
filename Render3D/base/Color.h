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
		 Color& operator =(const Color &_color);
		 Color operator +(Color _color) const;
		 Color& operator +=(Color _color);
		 Color operator *(Color _color) const;
		 Color operator *(float _color[3]) const;
		 Color operator -(Color _color) const;
	 };


	 static void ColorProduct(Color &c, const Color &a, const Color &b)
	 {
		 c.r = a.r * b.r;
		 c.g = a.g * b.g;
		 c.b = a.b * b.b;
		 c.a = a.a * b.a;
	 }

	 static void ColorProductArray(Color &c, const Color &a, const float *b)
	 {
		 c.r = a.r * b[0];
		 c.g = a.g * b[1];
		 c.b = a.b * b[2];
	 }
}