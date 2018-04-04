#include "Primitive.h"

namespace Render3D
{
	/*!
	* @function Render3D::Vertex::InitWithRhw
	*
	* @brief 顶点除以w
	*
	* @return void
	*
	*/
	void Vertex::InitWithRhw()
	{
		float rhw = 1.0f / pos.W();
		this->rhw = rhw;
		tc.u *= rhw;
		tc.v *= rhw;
		color.r *= rhw;
		color.g *= rhw;
		color.b *= rhw;
	}

	/*!
	* @function Render3D::Vertex::Interp
	*
	* @brief 两点之间插值
	*
	* @return void
	*
	* @param const Vertex x1
	* @param const Vertex x2
	* @param float t
	*/
	void Vertex::Interp(const Vertex x1, const Vertex x2, float t)
	{
		Math3D::VectorInterpolating(this->pos, x1.pos, x2.pos, t);
		this->tc.u = Math3D::Interp(x1.tc.u, x2.tc.u, t);
		this->tc.v = Math3D::Interp(x1.tc.v, x2.tc.v, t);
		this->color.r = Math3D::Interp(x1.color.r, x2.color.r, t);
		this->color.g = Math3D::Interp(x1.color.g, x2.color.g, t);
		this->color.b = Math3D::Interp(x1.color.b, x2.color.b, t);
		this->rhw = Math3D::Interp(x1.rhw, x2.rhw, t);
	}

	/*!
	* @function Render3D::Vertex::Division
	*
	* @brief 计算两点之间宽度为w的步长
	*
	* @return void
	*
	* @param const Vertex x1
	* @param const Vertex x2
	* @param float w
	*/
	void Vertex::Division(const Vertex x1, const Vertex x2, float w)
	{
		float inv = 1.0f / w;
		this->pos = (x2.pos - x1.pos) * inv;
		this->tc.u = (x2.tc.u - x1.tc.u) * inv;
		this->tc.v = (x2.tc.v - x1.tc.v) * inv;
		this->color.r = (x2.color.r - x1.color.r) * inv;
		this->color.g = (x2.color.g - x1.color.g) * inv;
		this->color.b = (x2.color.b - x1.color.b) * inv;
		this->rhw = (x2.rhw - x1.rhw) * inv;
	}

	/*!
	* @function Render3D::Vertex::Add
	*
	* @brief 点相加
	*
	* @return void
	*
	* @param const Vertex x
	*/
	void Vertex::Add(const Vertex x)
	{
		this->pos += x.pos;
		this->rhw += x.rhw;
		this->tc.u += x.tc.u;
		this->tc.v += x.tc.v;
		this->color.r += x.color.r;
		this->color.g += x.color.g;
		this->color.b += x.color.b;
	}

	/*!
	* @function Render3D::Trapezoid::InterpEdge
	*
	* @brief 按照Y坐标计算出左右两条边纵坐标等于Y的顶点
	*
	* @return void
	*
	* @param float y
	*/
	void Trapezoid::InterpEdge(float y)
	{
		float s1 = this->left.v2.pos.Y() - this->left.v1.pos.Y();
		float s2 = this->right.v2.pos.Y() - this->right.v1.pos.Y();
		float t1 = (y - this->left.v1.pos.Y()) / s1;
		float t2 = (y - this->right.v1.pos.Y()) / s2;
		this->left.v.Interp(this->left.v1, this->left.v2, t1);
		this->right.v.Interp(this->right.v1, this->right.v2, t2);
	}

	/*!
	* @function Render3D::Trapezoid::InitScanline 
	*
	* @brief 根据左右两边的端点,初始化计算出扫描线的起点和步长
	* 
	* @return void
	* 
	* @param Scanline & scanline
	* @param int y
	*/
	void Trapezoid::InitScanline(Scanline &scanline, int y)
	{
		float width = this->right.v.pos.X() - this->left.v.pos.X();
		scanline.x = (int)(this->left.v.pos.X() + 0.5f);
		scanline.w = (int)(this->right.v.pos.X() + 0.5f) - scanline.x;
		scanline.y = y;
		scanline.v = this->left.v;
		if (this->left.v.pos.X() >= this->right.v.pos.X())
			scanline.w = 0;
		scanline.step.Division(this->left.v, this->right.v, width);
	}

	/*!
	* @function Render3D::Trapezoids::operator[]
	*
	* @brief 获取Trapezoid
	*
	* @return Trapezoid&
	*
	* @param const int k
	*/
	Trapezoid& Trapezoids::operator [] (const int k)
	{
		assert(k <= 2 && k >= 0);
		return trap[k];
	}

	/*!
	* @function Render3D::Trapezoids::InitTriangle 
	*
	* @brief 根据水平线进行三角形切割
	* 
	* @return void
	* 
	* @param Vertex p1
	* @param Vertex p2
	* @param Vertex p3
	*/
	void Trapezoids::InitTriangle(Vertex &p1, Vertex &p2, Vertex &p3)
	{
		Vertex p;
		float k, x;

		if (p1.pos.Y() > p2.pos.Y()) p = p1, p1 = p2, p2 = p;
		if (p1.pos.Y() > p3.pos.Y()) p = p1, p1 = p3, p3 = p;
		if (p2.pos.Y() > p3.pos.Y()) p = p2, p2 = p3, p3 = p;
		if (p1.pos.Y() == p2.pos.Y() && p1.pos.Y() == p3.pos.Y())
		{
			count = 0;
			return;
		}
		if (p1.pos.X() == p2.pos.X() && p1.pos.X() == p3.pos.X())
		{
			count = 0;
			return;
		}

		if (p1.pos.Y() == p2.pos.Y())
		{
			if (p1.pos.X() > p2.pos.X()) p = p1, p1 = p2, p2 = p;
			trap[0].top = p1.pos.Y();
			trap[0].bottom = p3.pos.Y();
			trap[0].left.v1 = p1;
			trap[0].left.v2 = p3;
			trap[0].right.v1 = p2;
			trap[0].right.v2 = p3;
			count = (trap[0].top < trap[0].bottom) ? 1 : 0;
			return;
		}

		if (p2.pos.Y() == p3.pos.Y())
		{
			if (p2.pos.X() > p3.pos.X()) p = p2, p2 = p3, p3 = p;
			trap[0].top = p1.pos.Y();
			trap[0].bottom = p3.pos.Y();
			trap[0].left.v1 = p1;
			trap[0].left.v2 = p2;
			trap[0].right.v1 = p1;
			trap[0].right.v2 = p3;
			count = (trap[0].top < trap[0].bottom) ? 1 : 0;
			return;
		}

		trap[0].top = p1.pos.Y();
		trap[0].bottom = p2.pos.Y();
		trap[1].top = p2.pos.Y();
		trap[1].bottom = p3.pos.Y();

		k = (p3.pos.Y() - p1.pos.Y()) / (p2.pos.Y() - p1.pos.Y());
		x = p1.pos.X() + (p2.pos.X() - p1.pos.X()) * k;

		if (x <= p3.pos.X())
		{
			trap[0].left.v1 = p1;
			trap[0].left.v2 = p2;
			trap[0].right.v1 = p1;
			trap[0].right.v2 = p3;
			trap[1].left.v1 = p2;
			trap[1].left.v2 = p3;
			trap[1].right.v1 = p1;
			trap[1].right.v2 = p3;
		}
		else
		{
			trap[0].left.v1 = p1;
			trap[0].left.v2 = p3;
			trap[0].right.v1 = p1;
			trap[0].right.v2 = p2;
			trap[1].left.v1 = p1;
			trap[1].left.v2 = p3;
			trap[1].right.v1 = p2;
			trap[1].right.v2 = p3;
		}

		count = 2;
		return;
	}
}
