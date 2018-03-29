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
}