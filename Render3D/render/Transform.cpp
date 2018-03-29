#include "Transform.h"

namespace Render3D
{
	/*!
	* @function Render3D::Transform::Update
	*
	* @brief �任����ĸ���
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
	* @brief ���ֲ������µĵ�ͨ���任�������ˣ��任��CVV
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