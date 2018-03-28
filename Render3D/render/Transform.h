/*!
* @file Transform.h
* @date 2017/05/13 16:44
*
* @author QuincyKing
* Contact: j88172365@gmail.com
*
* @brief 几何阶段
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
		* @brief CVV裁剪
		*
		* @return int
		*
		* @param const Math3D::Vector4 & v
		*/
		static int CheckCVV(const Math3D::Vector4 &v)
		{
			//在透视投影之后，透视除法之前进行CVV裁剪.
			//检查点在CVV中的位置，这里的x-[-w, w], y-[-w, w], z-[0, w],该CVV与DirectX相似
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
		Math3D::Matrix44 model;         // 世界坐标变换
		Math3D::Matrix44 view;          // 摄影机坐标变换
		Math3D::Matrix44 viewR;
		Math3D::Matrix44 projection;    // 投影变换
		Math3D::Matrix44 vp;
		Math3D::Matrix44 mv;
		Math3D::Matrix44 mvp;
	};
}