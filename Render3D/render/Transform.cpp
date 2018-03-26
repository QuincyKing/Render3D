#include "Transform.h"

namespace Render3D
{
	/*!
	* @function Render3D::Transform::Update
	*
	* @brief 变换矩阵的更新
	*
	* @return void
	*
	*/
	void Transform::Update()
	{
		mv = model * view;
		vp = view * projection;
		mvp = mv * projection;
	}

	/*!
	* @function Render3D::Transform::Apply
	*
	* @brief 将局部坐标下的点通过变换矩阵的相乘，变换到CVV
	*
	* @return void
	*
	* @param Math3D::Vector4 & y
	* @param const Math3D::Vector4 & x
	*/
	void Transform::Apply(Math3D::Vector4 &y, const Math3D::Vector4 &x)
	{
		y = x * mvp;
	}

	/*!
	* @function Render3D::Transform::Homogenize
	*
	* @brief 透视除法
	*
	* @return void
	*
	* @param Math3D::Vector4 & y
	* @param const Math3D::Vector4 & x
	*/
	void Transform::Homogenize(Math3D::Vector4 &y, const Math3D::Vector4 &x, float width, float height)
	{
		float rhw = 1.0f / x.W();
		y.X() = (x.X() * rhw + 1.0f) * width * 0.5f;
		y.Y() = (1.0f - x.Y() * rhw) * height * 0.5f;
		y.Z() = x.Z() * rhw;
		y.W() = 1.0f;
	}

	void Transform::HomogenizeReverse(Math3D::Vector4 &y, const Math3D::Vector4 &x, float w, float width, float height)
	{
		y.X() = (x.X() * 2 / width - 1.0f) * w;
		y.Y() = (1.0f - x.Y() * 2 / height) * w;
		y.Z() = x.Z() * w;
		y.W() = w;
	}

	void Transform::SetModel(const Math3D::Matrix44 &_matrix)
	{
		model = _matrix;
	}

	void Transform::SetView(const Math3D::Matrix44 &_matrix)
	{
		view = _matrix;
	}

	void Transform::SetViewR(const Math3D::Matrix44 &_matrix)
	{
		viewR = _matrix;
	}

	void Transform::SetProjection(const Math3D::Matrix44 &_matrix)
	{
		projection = _matrix;
	}
}