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
		void Apply(Math3D::Vector4 &y, const Math3D::Vector4 &x);
		
		void SetModel(const Math3D::Matrix44 &_matrix);
		void SetView(const Math3D::Matrix44 &_matrix);
		void SetViewR(const Math3D::Matrix44 &_matrix);
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
		Math3D::Matrix44 model;         // ��������任
		Math3D::Matrix44 view;          // ��Ӱ������任
		Math3D::Matrix44 viewR;
		Math3D::Matrix44 projection;    // ͶӰ�任
		Math3D::Matrix44 vp;
		Math3D::Matrix44 mv;
		Math3D::Matrix44 mvp;
	};
}