#pragma once

namespace Base3D
{
     class Color
	 {
	 public:
		 float r, g, b, a;

		 Color();
		 Color operator *(float scale) const;
		 Color& operator =(Color &_color);
		 Color operator +(Color _color) const;
		 Color& operator +=(Color _color);
		 Color operator *(Color _color) const;
		 Color operator *(float _color[3]) const;
		 Color operator -(Color _color) const;
		 static void Interpolating(Color &res, const Color &src1, const Color &src2, const Color &src3
			 , float a, float b, float c);
	 };

	 Color::Color()
	 {
		 r = 0.0;
		 g = 0.0; 
		 b = 0.0;
		 a = 0.0;
	 }

	 Color Color::operator*(float _scale) const
	 {
		 Color color;
		 color.r = this->r * _scale;
		 color.g = this->g * _scale;
		 color.b = this->b * _scale;
		 color.a = this->a * _scale;
		 return color;
	 }

	 Color& Color::operator =(Color &_color)
	 {
		 this->r = _color.r;
		 this->g = _color.g;
		 this->b = _color.b;
		 this->a = _color.a;
		 return *this;
	 }

	 Color Color::operator+ (const Color _color) const
	 {
		 Color color;
		 color.r  = this->r + _color.r;
		 color.g = this->g + _color.g;
		 color.b = this->b + _color.b;
		 color.a = this->a + _color.a;
		 return  color;
	 }

	 Color& Color::operator +=(Color _color)
	 {
		 this->a += _color.a;
		 this->b += _color.b;
		 this->g += _color.g;
		 this->r += _color.r;
		 return *this;
	 }

	 Color Color::operator *(const Color _color) const
	 {
		 Color color;
		 color.r = this->r * _color.r;
		 color.g = this->g * _color.g;
		 color.b = this->b * _color.b;
		 color.a = this->a * _color.a;
		 return color;
	 }

	 Color Color::operator *(float _color[3]) const
	 {
		 Color color;
		 color.r = this->r * _color[0];
		 color.g = this->g * _color[1];
		 color.b = this->b * _color[2];
		 return color;
	 }
     
	 Color Color::operator-(Color _color) const
	 {
		 Color color;
		 color.r = this->r - _color.r;
		 color.g = this->g - _color.g;
		 color.b = this->b - _color.b;
		 color.a = this->a - _color.a;

		 return color;
	 }

	 void Color::Interpolating(Color &res, const Color &src1, const Color &src2, const Color &src3
		 , float a, float b, float c)
	 {
		 Color tmp;
		 tmp = src1 * a;
		 tmp += src2 * b;
		 tmp += src3 * c;
		 res = tmp;
	 }
}


