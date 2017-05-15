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
		//��World��View, Projection����任�󣬽��б任����ĸ��£�
		Math3D::Matrix44 m;
		m = m_World * m_View;
		m_Transform = m * m_Projection;
	}

	/*!
	* @function Render3D::Transform::Init
	*
	* @brief ��ʼ��
	*
	* @return void
	*
	* @param int _width
	* @param int _height
	*/
	void Transform::Init(int _width, int _height)
	{
		//1.World����Identity;
		//2.View����Indentity;
		//3.������Ļ�ĳ���ȣ��Լ�Ĭ���ӽ�Ϊ90�ȣ����ü���Ϊ1.0f,Զ�ü���Ϊ500.0f
		//4.�������
		float aspect = (float)_width / ((float)_height);

		m_World.Identity();
		m_View.Identity();
		MatrixSetPerspective(m_Projection, 3.1415926f * 0.5f, aspect, 1.0f, 500.0f);
		w = (float)_width;
		h = (float)_height;
		Update();
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
		y = x * m_Transform;
	}

	/*!
	* @function Render3D::Transform::Homogenize
	*
	* @brief ͸�ӳ���
	*
	* @return void
	*
	* @param Math3D::Vector4 & y
	* @param const Math3D::Vector4 & x
	*/
	void Transform::Homogenize(Math3D::Vector4 &y, const Math3D::Vector4 &x)
	{
		float rhw = 1.0f / x.W();
		y.X() = (x.X() * rhw + 1.0f) * w * 0.5f;
		y.Y() = (1.0f - x.Y() * rhw) * h * 0.5f;
		y.Z() = x.Z() * rhw;
		y.W() = 1.0f;
	}

	void Transform::SetWorld(const Math3D::Matrix44 &_matrix)
	{
		m_World = _matrix;
	}

	void Transform::SetView(const Math3D::Matrix44 &_matrix)
	{
		m_View = _matrix;
	}

	void Transform::SetProjection(const Math3D::Matrix44 &_matrix)
	{
		m_Projection = _matrix;
	}
}