/*!
* @file Transform.h
* @date 2017/05/13 16:44
*
* @author QuincyKing
* Contact: j88172365@gmail.com
*
* @brief ���ν׶�
*/
#pragma  once

#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "../math/MathUtil.h"

namespace Render3D
{
	class Transform
	{
	public:
		void Update();
		void Init(int _width, int _height);
		void Apply(Math3D::Vector4 &y, const Math3D::Vector4 &x);
		void Homogenize(Math3D::Vector4 &y, const Math3D::Vector4 &x);

		void SetWorld(const Math3D::Matrix44 &_matrix);
		void SetView(const Math3D::Matrix44 &_matrix);
		void SetProjection(const Math3D::Matrix44 &_matrix);
		/*!
		* @function Transform::CheckCVV
		*
		* @brief CVV�ü�
		*
		* @return int
		*
		* @param const Math3D::Vector4 & v
		*/
		static int CheckCVV(const Math3D::Vector4 &v)
		{
			//��͸��ͶӰ֮��͸�ӳ���֮ǰ����CVV�ü�.
			//������CVV�е�λ�ã������x-[-w, w], y-[-w, w], z-[0, w],��CVV��DirectX����
			float w = v.W();
			int check = 0;
			if (v.Z() < 0.0f)	check |= 1;
			if (v.Z() > w)		check |= 2;
			if (v.X() < -w)		check |= 4;
			if (v.X() > w)		check |= 8;
			if (v.Y() < -w)		check |= 16;
			if (v.Y() > w)		check |= 32;
			return check;
		}

	private:
		Math3D::Matrix44 m_World;         // ��������任
		Math3D::Matrix44 m_View;          // ��Ӱ������任
		Math3D::Matrix44 m_Projection;    // ͶӰ�任
		Math3D::Matrix44 m_Transform;     // transform = world * view * projection
		float w, h;						  // ��Ļ��С
	};
}