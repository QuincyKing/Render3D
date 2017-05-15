/*!
* @file MathUtil.h
* @date 2017/05/13 18:07
*
* @author QuincyKing
* Contact: j88172365@gmail.com
*
* @brief 数学工具类
*/
#pragma once

#include "Matrix.h"
#include "Vector.h"
#include <cmath>

namespace Math3D
{
	template<typename Real>
	inline Vector<Real, 4> operator* (const Vector<Real, 4>& vec, const Matrix<Real>& _mat)
	{
		return Vector<Real, 4>(
			vec[0] * _mat.M11 + vec[1] * _mat.M21 + vec[2] * _mat.M31 + vec[3] * _mat.M41,
			vec[0] * _mat.M12 + vec[1] * _mat.M22 + vec[2] * _mat.M32 + vec[3] * _mat.M42,
			vec[0] * _mat.M13 + vec[1] * _mat.M23 + vec[2] * _mat.M33 + vec[3] * _mat.M43,
			vec[0] * _mat.M14 + vec[1] * _mat.M24 + vec[2] * _mat.M34 + vec[3] * _mat.M44);
	}

	template<typename Real>
	inline Vector<Real, 4> operator* (const Matrix<Real>& _mat, const Vector<Real, 4>& vec)
	{
		return Vector<Real, 4>(
			_mat.M11*vec[0] + _mat.M12*vec[1] + _mat.M13*vec[2] + _mat.M14*vec[3],
			_mat.M21*vec[0] + _mat.M22*vec[1] + _mat.M23*vec[2] + _mat.M24*vec[3],
			_mat.M31*vec[0] + _mat.M32*vec[1] + _mat.M33*vec[2] + _mat.M34*vec[3],
			_mat.M41*vec[0] + _mat.M42*vec[1] + _mat.M43*vec[2] + _mat.M44*vec[3]);
	}

	template<typename Real>
	inline Matrix<Real> operator*(const Matrix<Real>& M1, Matrix<Real>& M2)
	{
		Matrix<Real> result;

		float x = M1.M11;
		float y = M1.M12;
		float z = M1.M13;
		float w = M1.M14;

		result.M11 = (M2.M11*x) + (M2.M21*y) + (M2.M31*z) + (M2.M41*w);
		result.M12 = (M2.M12*x) + (M2.M22*y) + (M2.M32*z) + (M2.M42*w);
		result.M13 = (M2.M13*x) + (M2.M23*y) + (M2.M33*z) + (M2.M43*w);
		result.M14 = (M2.M14*x) + (M2.M24*y) + (M2.M34*z) + (M2.M44*w);

		x = M1.M21;
		y = M1.M22;
		z = M1.M23;
		w = M1.M24;
		result.M21 = (M2.M11*x) + (M2.M21*y) + (M2.M31*z) + (M2.M41*w);
		result.M22 = (M2.M12*x) + (M2.M22*y) + (M2.M32*z) + (M2.M42*w);
		result.M23 = (M2.M13*x) + (M2.M23*y) + (M2.M33*z) + (M2.M43*w);
		result.M24 = (M2.M14*x) + (M2.M24*y) + (M2.M34*z) + (M2.M44*w);

		x = M1.M31;
		y = M1.M32;
		z = M1.M33;
		w = M1.M34;
		result.M31 = (M2.M11*x) + (M2.M21*y) + (M2.M31*z) + (M2.M41*w);
		result.M32 = (M2.M12*x) + (M2.M22*y) + (M2.M32*z) + (M2.M42*w);
		result.M33 = (M2.M13*x) + (M2.M23*y) + (M2.M33*z) + (M2.M43*w);
		result.M34 = (M2.M14*x) + (M2.M24*y) + (M2.M34*z) + (M2.M44*w);

		x = M1.M41;
		y = M1.M42;
		z = M1.M43;
		w = M1.M44;
		result.M41 = (M2.M11*x) + (M2.M21*y) + (M2.M31*z) + (M2.M41*w);
		result.M42 = (M2.M12*x) + (M2.M22*y) + (M2.M32*z) + (M2.M42*w);
		result.M43 = (M2.M13*x) + (M2.M23*y) + (M2.M33*z) + (M2.M43*w);
		result.M44 = (M2.M14*x) + (M2.M24*y) + (M2.M34*z) + (M2.M44*w);

		return result;
	}

	template<typename Real>
	Matrix<Real> MatrixMultiply(const Matrix<Real>& M1, Matrix<Real>& M2)
	{
		Matrix<Real> result;

		float x = M1.M11;
		float y = M1.M12;
		float z = M1.M13;
		float w = M1.M14;

		result.M11 = (M2.M11*x) + (M2.M21*y) + (M2.M31*z) + (M2.M41*w);
		result.M12 = (M2.M12*x) + (M2.M22*y) + (M2.M32*z) + (M2.M42*w);
		result.M13 = (M2.M13*x) + (M2.M23*y) + (M2.M33*z) + (M2.M43*w);
		result.M14 = (M2.M14*x) + (M2.M24*y) + (M2.M34*z) + (M2.M44*w);

		x = M1.M21;
		y = M1.M22;
		z = M1.M23;
		w = M1.M24;
		result.M21 = (M2.M11*x) + (M2.M21*y) + (M2.M31*z) + (M2.M41*w);
		result.M22 = (M2.M12*x) + (M2.M22*y) + (M2.M32*z) + (M2.M42*w);
		result.M23 = (M2.M13*x) + (M2.M23*y) + (M2.M33*z) + (M2.M43*w);
		result.M24 = (M2.M14*x) + (M2.M24*y) + (M2.M34*z) + (M2.M44*w);

		x = M1.M31;
		y = M1.M32;
		z = M1.M33;
		w = M1.M34;
		result.M31 = (M2.M11*x) + (M2.M21*y) + (M2.M31*z) + (M2.M41*w);
		result.M32 = (M2.M12*x) + (M2.M22*y) + (M2.M32*z) + (M2.M42*w);
		result.M33 = (M2.M13*x) + (M2.M23*y) + (M2.M33*z) + (M2.M43*w);
		result.M34 = (M2.M14*x) + (M2.M24*y) + (M2.M34*z) + (M2.M44*w);

		x = M1.M41;
		y = M1.M42;
		z = M1.M43;
		w = M1.M44;
		result.M41 = (M2.M11*x) + (M2.M21*y) + (M2.M31*z) + (M2.M41*w);
		result.M42 = (M2.M12*x) + (M2.M22*y) + (M2.M32*z) + (M2.M42*w);
		result.M43 = (M2.M13*x) + (M2.M23*y) + (M2.M33*z) + (M2.M43*w);
		result.M44 = (M2.M14*x) + (M2.M24*y) + (M2.M34*z) + (M2.M44*w);

		return result;
	}

	template<typename Real>
	Matrix<Real> MatrixInverse(const Matrix<Real>& _mat)
	{
		Real v0 = _mat.M31 * _mat.M42 - _mat.M32 * _mat.M41;
		Real v1 = _mat.M31 * _mat.M43 - _mat.M33 * _mat.M41;
		Real v2 = _mat.M31 * _mat.M44 - _mat.M34 * _mat.M41;
		Real v3 = _mat.M32 * _mat.M43 - _mat.M33 * _mat.M42;
		Real v4 = _mat.M32 * _mat.M44 - _mat.M34 * _mat.M42;
		Real v5 = _mat.M33 * _mat.M44 - _mat.M34 * _mat.M43;

		Real i11 = (v5 * _mat.M22 - v4 * _mat.M23 + v3 * _mat.M24);
		Real i21 = -(v5 * _mat.M21 - v2 * _mat.M23 + v1 * _mat.M24);
		Real i31 = (v4 * _mat.M21 - v2 * _mat.M22 + v0 * _mat.M24);
		Real i41 = -(v3 * _mat.M21 - v1 * _mat.M22 + v0 * _mat.M23);

		Real invDet = 1.0f / (i11 * _mat.M11 + i21 * _mat.M12 + i31 * _mat.M13 + i41 * _mat.M14);

		i11 *= invDet;
		i21 *= invDet;
		i31 *= invDet;
		i41 *= invDet;

		Real i12 = -(v5 * _mat.M12 - v4 * _mat.M13 + v3 * _mat.M14) * invDet;
		Real i22 = (v5 * _mat.M11 - v2 * _mat.M13 + v1 * _mat.M14) * invDet;
		Real i32 = -(v4 * _mat.M11 - v2 * _mat.M12 + v0 * _mat.M14) * invDet;
		Real i42 = (v3 * _mat.M11 - v1 * _mat.M12 + v0 * _mat.M13) * invDet;

		v0 = _mat.M21 * _mat.M42 - _mat.M22 * _mat.M41;
		v1 = _mat.M21 * _mat.M43 - _mat.M23 * _mat.M41;
		v2 = _mat.M21 * _mat.M44 - _mat.M24 * _mat.M41;
		v3 = _mat.M22 * _mat.M43 - _mat.M23 * _mat.M42;
		v4 = _mat.M22 * _mat.M44 - _mat.M24 * _mat.M42;
		v5 = _mat.M23 * _mat.M44 - _mat.M24 * _mat.M43;

		Real i13 = (v5 * _mat.M12 - v4 * _mat.M13 + v3 * _mat.M14) * invDet;
		Real i23 = -(v5 * _mat.M11 - v2 * _mat.M13 + v1 * _mat.M14) * invDet;
		Real i33 = (v4 * _mat.M11 - v2 * _mat.M12 + v0 * _mat.M14) * invDet;
		Real i43 = -(v3 * _mat.M11 - v1 * _mat.M12 + v0 * _mat.M13) * invDet;

		v0 = _mat.M32 * _mat.M21 - _mat.M31 * _mat.M22;
		v1 = _mat.M33 * _mat.M21 - _mat.M31 * _mat.M23;
		v2 = _mat.M34 * _mat.M21 - _mat.M31 * _mat.M24;
		v3 = _mat.M33 * _mat.M22 - _mat.M32 * _mat.M23;
		v4 = _mat.M34 * _mat.M22 - _mat.M32 * _mat.M24;
		v5 = _mat.M34 * _mat.M23 - _mat.M33 * _mat.M24;

		Real i14 = -(v5 * _mat.M12 - v4 * _mat.M13 + v3 * _mat.M14) * invDet;
		Real i24 = (v5 * _mat.M11 - v2 * _mat.M13 + v1 * _mat.M14) * invDet;
		Real i34 = -(v4 * _mat.M11 - v2 * _mat.M12 + v0 * _mat.M14) * invDet;
		Real i44 = (v3 * _mat.M11 - v1 * _mat.M12 + v0 * _mat.M13) * invDet;

		return Matrix<Real>(
			i11, i12, i13, i14,
			i21, i22, i23, i24,
			i31, i32, i33, i34,
			i41, i42, i43, i44);
	}

	template<typename Real>
	inline Matrix<Real> MatrixTranspose(const Matrix<Real>& _mat)
	{
		return Matrix<Real>(_mat.Elements[0], _mat.Elements[4], _mat.Elements[8], _mat.Elements[12],
			_mat.Elements[1], _mat.Elements[5], _mat.Elements[9], _mat.Elements[13],
			_mat.Elements[2], _mat.Elements[6], _mat.Elements[10], _mat.Elements[14],
			_mat.Elements[3], _mat.Elements[7], _mat.Elements[11], _mat.Elements[15]);
	}

	template<typename Real>
	Real MatrixDeterminant(const Matrix<Real>& _mat)
	{
		Real fA0 = _mat.Elements[0] * _mat.Elements[5] - _mat.Elements[1] * _mat.Elements[4];
		Real fA1 = _mat.Elements[0] * _mat.Elements[6] - _mat.Elements[2] * _mat.Elements[4];
		Real fA2 = _mat.Elements[0] * _mat.Elements[7] - _mat.Elements[3] * _mat.Elements[4];
		Real fA3 = _mat.Elements[1] * _mat.Elements[6] - _mat.Elements[2] * _mat.Elements[5];
		Real fA4 = _mat.Elements[1] * _mat.Elements[7] - _mat.Elements[3] * _mat.Elements[5];
		Real fA5 = _mat.Elements[2] * _mat.Elements[7] - _mat.Elements[3] * _mat.Elements[6];
		Real fB0 = _mat.Elements[8] * _mat.Elements[13] - _mat.Elements[9] * _mat.Elements[12];
		Real fB1 = _mat.Elements[8] * _mat.Elements[14] - _mat.Elements[10] * _mat.Elements[12];
		Real fB2 = _mat.Elements[8] * _mat.Elements[15] - _mat.Elements[11] * _mat.Elements[12];
		Real fB3 = _mat.Elements[9] * _mat.Elements[14] - _mat.Elements[10] * _mat.Elements[13];
		Real fB4 = _mat.Elements[9] * _mat.Elements[15] - _mat.Elements[11] * _mat.Elements[13];
		Real fB5 = _mat.Elements[10] * _mat.Elements[15] - _mat.Elements[11] * _mat.Elements[14];
		Real fDet = fA0*fB5 - fA1*fB4 + fA2*fB3 + fA3*fB2 - fA4*fB1 + fA5*fB0;
		return fDet;
	}

	template <class Real, int Size>
	inline Vector<Real, Size> operator* (Real fScalar, const Vector<Real, Size>& rhs)
	{
		Vector<Real, Size> result;
		for (int i = 0; i < Size; i++)
			result[i] = fScalar * rhs[i];

		return result;
	}

	template< typename Real, int Size >
	inline Real Length(const Vector<Real, Size>& vec)
	{
		Real result = Real(0);
		for (int i = 0; i < Size; i++)
			result += vec[i] * vec[i];
		return sqrt(result);
	}

	template< typename Real, int Size >
	inline Real LengthSquared(const Vector<Real, Size>& vec)
	{
		Real result = Real(0);
		for (int i = 0; i < Size-1; i++)
			result += vec[i] * vec[i];
		return result;
	}

	template< typename Real, int Size >
	inline Real Dot(const Vector<Real, Size>& lfs, const Vector<Real, Size>& rhs)
	{
		Real result = Real(0);
		for (int i = 0; i < Size; i++)
			result += lfs[i] * rhs[i];
		return result;
	}

	template< typename Real, int Size >
	inline Vector<Real, Size> Normalize(const Vector<Real, Size>& vec)
	{
		Real fLength = LengthSquared(vec);
		Real fInvScalar = ((Real)1.0) / ((Real)sqrt((double)fLength));
		return vec * fInvScalar;
	}

	template< typename Real, int Size >
	inline Vector<Real, Size> Lerp(const Vector<Real, Size>& vec1, const Vector<Real, Size>& vec2, float t)
	{
		Vector<Real, Size> retVal;

		for (uint32_t i = 0; i < Size; ++i)
		{
			retVal[i] = vec1[i] + (vec2[i] - vec1[i]) * t;
		}

		return retVal;
	}

	template<typename Real>
	inline Vector<Real, 3> Cross(const Vector<Real, 3>& vec1, const Vector<Real, 3>& vec2)
	{
		return Vector<Real, 3>(
			vec1.Y() * vec2.Z() - vec1.Z() * vec2.Y(),
			vec1.Z() * vec2.X() - vec1.X() * vec2.Z(),
			vec1.X() * vec2.Y() - vec1.Y() * vec2.X());
	}

	template<typename Real>
	inline Vector<Real, 4> Cross(const Vector<Real, 4>& vec1, const Vector<Real, 4>& vec2)
	{
		return Vector<Real, 4>(
			vec1.Y() * vec2.Z() - vec1.Z() * vec2.Y(),
			vec1.Z() * vec2.X() - vec1.X() * vec2.Z(),
			vec1.X() * vec2.Y() - vec1.Y() * vec2.X(),
			Real(0));
	}

	static int CMID(int x, int min, int max)
	{
		return (x < min) ? min : ((x > max) ? max : x);
	}

	static float Interp(float x1, float x2, float t)
	{
		return x1 + (x2 - x1) * t;
	}

	static void VectorInterp(Vector4 &z, const Vector4 x1, const Vector4 x2, float t)
	{
		z[0] = Interp(x1.X(), x2.X(), t);
		z[1] = Interp(x1.Y(), x2.Y(), t);
		z[2] = Interp(x1.Z(), x2.Z(), t);
		z[3] = 1.0f;
	}

	static void MatrixSetTranslate(Matrix44 &m, float x, float y, float z)
	{
		m.Identity();
		m.M41 = x;
		m.M42 = y;
		m.M43 = z;
	}

	static void MatrixSetScale(Matrix44 &m, float x, float y, float z)
	{
		m.Identity();
		m.M11 = x;
		m.M22 = y;
		m.M33 = z;
	}

	static void MatrixSetRotate(Matrix44 &m, float x, float y, float z, float theta)
	{
		float qsin = (float)sin(theta * 0.5f);
		float qcos = (float)cos(theta * 0.5f);
		Vector4 vec = { x, y, z, 1.0f };
		float w = qcos;
		vec = Normalize(vec);
		x = vec.X() * qsin;
		y = vec.Y() * qsin;
		z = vec.Z() * qsin;
		m.M11 = 1 - 2 * y * y - 2 * z * z;
		m.M21 = 2 * x * y - 2 * w * z;
		m.M31 = 2 * x * z + 2 * w * y;
		m.M12 = 2 * x * y + 2 * w * z;
		m.M22 = 1 - 2 * x * x - 2 * z * z;
		m.M32 = 2 * y * z - 2 * w * x;
		m.M13 = 2 * x * z - 2 * w * y;
		m.M23 = 2 * y * z + 2 * w * x;
		m.M33 = 1 - 2 * x * x - 2 * y * y;
		m.M14 = m.M24 = m.M34 = 0.0f;
		m.M41 = m.M42 = m.M43 = 0.0f;
		m.M44 = 1.0f;
	}

	//UVN相机模型
	static void MatrixSetLookat(Matrix44 &m, const  Vector4 &eye, const  Vector4 &at, const  Vector4 &up)
	{
		Vector4 xaxis, yaxis, zaxis;

		zaxis = at - eye;
		zaxis = Normalize(zaxis);
		xaxis = Cross(up, zaxis);
		xaxis = Normalize(xaxis);
		yaxis = Cross(zaxis, xaxis);

		m.M11 = xaxis.X();
		m.M21 = xaxis.Y();
		m.M31 = xaxis.Z();
		m.M41 = -Dot(xaxis, eye);

		m.M12 = yaxis.X();
		m.M22 = yaxis.Y();
		m.M32 = yaxis.Z();
		m.M42 = -Dot(yaxis, eye);

		m.M13 = zaxis.X();
		m.M23 = zaxis.Y();
		m.M33 = zaxis.Z();
		m.M43 = -Dot(zaxis, eye);

		m.M14 = m.M24 = m.M34 = 0.0f;
		m.M44 = 1.0f;
	}

	static void MatrixSetPerspective(Matrix44 &m, float fovy, float aspect, float zn, float zf)
	{
		float fax = 1.0f / (float)tan(fovy * 0.5f);
		m.MakeZero();
		m.M11 = (float)(fax / aspect);
		m.M22 = (float)(fax);
		m.M33 = zf / (zf - zn);
		m.M43 = -zn * zf / (zf - zn);
		m.M34 = 1;
	}
}