/*!
* @file Matrix.h
* @date 2017/05/13 17:03
*
* @author QuincyKing
* Contact: j88172365@gmail.com
*
* @brief æÿ’Û¿‡
*/
#pragma once

#include <cstring>
#include "Vector.h"

namespace Math3D
{
	#define MatrixItem(iRow, iCol) ( iRow*4 + iCol )

	template<typename Real>
	class Matrix
	{
	public:
		typedef Real value_type;

	public:
		Matrix();
		Matrix(const Real* _rhs);
		Matrix(Real _m11, Real _m12, Real _m13, Real _m14,
			   Real _m21, Real _m22, Real _m23, Real _m24,
			   Real _m31, Real _m32, Real _m33, Real _m34,
			   Real _m41, Real _m42, Real _m43, Real _m44);
		template<typename T>
		Matrix(const Matrix<T>& _rhs);
		void MakeIdentity();
		void MakeZero();
		inline const Real* operator() () const					{ return m_Elements; }
		inline void operator =();
		inline Real* operator() ()								{ return m_Elements; }
		inline Real operator[] (int index) const				{ return m_Elements[index]; }
		inline Real& operator[] (int index)						{ return m_Elements[index]; }
		inline Real operator() (int iRow, int iCol) const		{ return m_Elements[MatrixItem(iRow, iCol)]; }
		inline Real& operator() (int iRow, int iCol)			{ return m_Elements[MatrixItem(iRow, iCol)]; }

		void SetRow(int _iRow, const Vector<Real, 4>& _rhs);
		Vector<Real, 4> GetRow(int _iRow) const;
		void SetColumn(int _iCol, const Vector<Real, 4>& _rhs);
		Vector<Real, 4> GetColumn(int _iCol) const;
		bool operator== (const Matrix& _rhs) const;
		bool operator!= (const Matrix& _rhs) const;
		Matrix operator+ (const Matrix& _rhs) const;
		Matrix operator- (const Matrix& _rhs) const;
		Matrix operator* (Real _scalar) const;
		Matrix operator/ (Real _scalar) const;
		Matrix operator- () const;
		Matrix& operator+= (const Matrix& _rhs);
		Matrix& operator-= (const Matrix& _rhs);
		Matrix& operator*= (Real _scalar);
		Matrix& operator/= (Real _scalar);
		static Matrix& Identity();

	public:
		union
		{
			struct{ Real m_Elements[16]; };
			struct
			{
				Real M11, M12, M13, M14;
				Real M21, M22, M23, M24;
				Real M31, M32, M33, M34;
				Real M41, M42, M43, M44;
			};
		};
	};

template<typename Real>
Matrix<Real>::Matrix()
{
	MakeZero();
}

template<typename Real>
Matrix<Real>::Matrix(const Real* _rhs)
{
	memcpy(m_Elements, _rhs, sizeof(m_Elements));
}

template<typename Real>
Matrix<Real>::Matrix(Real _m11, Real _m12, Real _m13, Real _m14,
					 Real _m21, Real _m22, Real _m23, Real _m24,
					 Real _m31, Real _m32, Real _m33, Real _m34,
					 Real _m41, Real _m42, Real _m43, Real _m44)
{
	M11 = _m11; M12 = _m12; M13 = _m13; M14 = _m14;
	M21 = _m21; M22 = _m22; M23 = _m23; M24 = _m24;
	M31 = _m31; M32 = _m32; M33 = _m33; M34 = _m34;
	M41 = _m41; M42 = _m42; M43 = _m43; M44 = _m44;
}

template<typename Real>
template<typename T>
Matrix<Real>::Matrix(const Matrix<T>& _rhs)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			m_Elements[MatrixItem(i, j)] = static_cast<Real>(_rhs.m_Elements[MatrixItem(i, j)]);
		}
}

template<typename Real>
void Matrix<Real>::MakeIdentity()
{
	m_Elements[0] = (Real)1.0; m_Elements[1] = (Real)0.0; m_Elements[2] = (Real)0.0; m_Elements[3] = (Real)0.0;
	m_Elements[4] = (Real)0.0; m_Elements[5] = (Real)1.0; m_Elements[6] = (Real)0.0; m_Elements[7] = (Real)0.0;
	m_Elements[8] = (Real)0.0; m_Elements[9] = (Real)0.0; m_Elements[10] = (Real)1.0; m_Elements[11] = (Real)0.0;
	m_Elements[12] = (Real)0.0; m_Elements[13] = (Real)0.0; m_Elements[14] = (Real)0.0; m_Elements[15] = (Real)1.0;
}

template<typename Real>
void Matrix<Real>::MakeZero()
{
	memset(m_Elements, 0, sizeof(m_Elements));
}

template<typename Real>
void Matrix<Real>::SetRow(int _iRow, const Vector<Real, 4>& _rhs)
{
	m_Elements[MatrixItem(_iRow, 0)] = _rhs[0];
	m_Elements[MatrixItem(_iRow, 1)] = _rhs[1];
	m_Elements[MatrixItem(_iRow, 2)] = _rhs[2];
	m_Elements[MatrixItem(_iRow, 3)] = _rhs[3];
}

template<typename Real>
Vector<Real, 4> Matrix<Real>::GetRow(int _iRow) const
{
	return Vector<Real, 4>(m_Elements[MatrixItem(_iRow, 0)], m_Elements[MatrixItem(_iRow, 1)],
		m_Elements[MatrixItem(_iRow, 2)], m_Elements[MatrixItem(_iRow, 3)]);
}

template<typename Real>
void Matrix<Real>::SetColumn(int _iCol, const Vector<Real, 4>& _rhs)
{
	m_Elements[MatrixItem(0, _iCol)] = _rhs[0];
	m_Elements[MatrixItem(1, _iCol)] = _rhs[1];
	m_Elements[MatrixItem(2, _iCol)] = _rhs[2];
	m_Elements[MatrixItem(3, _iCol)] = _rhs[3];
}

template<typename Real>
Vector<Real, 4> Matrix<Real>::GetColumn(int _iCol) const
{
	return Vector<Real, 4>(m_Elements[MatrixItem(0, _iCol)], m_Elements[MatrixItem(1, _iCol)],
		m_Elements[MatrixItem(2, _iCol)], m_Elements[MatrixItem(3, _iCol)]);
}

template<typename Real>
void Matrix<Real>::operator =(const Matrix<Real>& _rhs)
{
	M11 = _rhs.M11; M21 = _rhs.M21; M31 = _rhs.M31; M41 = _rhs.M41;
	M12 = _rhs.M12; M22 = _rhs.M22; M32 = _rhs.M32; M42 = _rhs.M42;
	M13 = _rhs.M13; M23 = _rhs.M23; M33 = _rhs.M33; M43 = _rhs.M43;
	M14 = _rhs.M14; M24 = _rhs.M24; M34 = _rhs.M34; M44 = _rhs.M44;
}

template<typename Real>
bool Matrix<Real>::operator== (const Matrix<Real>& _rhs) const
{
	return memcmp(m_Elements, _rhs.m_Elements, sizeof(m_Elements)) == 0;
}

template<typename Real>
bool Matrix<Real>::operator!= (const Matrix<Real>& _rhs) const
{
	return memcmp(m_Elements, _rhs.m_Elements, sizeof(m_Elements)) != 0;
}

template<typename Real>
Matrix<Real> Matrix<Real>::operator+ (const Matrix<Real>& _rhs) const
{
	return Matrix<Real>(
		m_Elements[0] + _rhs.m_Elements[0], m_Elements[1] + _rhs.m_Elements[1], m_Elements[2] + _rhs.m_Elements[2], m_Elements[3] + _rhs.m_Elements[3],
		m_Elements[4] + _rhs.m_Elements[4], m_Elements[5] + _rhs.m_Elements[5], m_Elements[6] + _rhs.m_Elements[6], m_Elements[7] + _rhs.m_Elements[7],
		m_Elements[8] + _rhs.m_Elements[8], m_Elements[9] + _rhs.m_Elements[9], m_Elements[10] + _rhs.m_Elements[10], m_Elements[11] + _rhs.m_Elements[11],
		m_Elements[12] + _rhs.m_Elements[12], m_Elements[13] + _rhs.m_Elements[13], m_Elements[14] + _rhs.m_Elements[14], m_Elements[15] + _rhs.m_Elements[15]
		);
}

template<typename Real>
Matrix<Real> Matrix<Real>::operator- (const Matrix<Real>& _rhs) const
{
	return Matrix<Real>(
		m_Elements[0] - _rhs.m_Elements[0], m_Elements[1] - _rhs.m_Elements[1], m_Elements[2] - _rhs.m_Elements[2], m_Elements[3] - _rhs.m_Elements[3],
		m_Elements[4] - _rhs.m_Elements[4], m_Elements[5] - _rhs.m_Elements[5], m_Elements[6] - _rhs.m_Elements[6], m_Elements[7] - _rhs.m_Elements[7],
		m_Elements[8] - _rhs.m_Elements[8], m_Elements[9] - _rhs.m_Elements[9], m_Elements[10] - _rhs.m_Elements[10], m_Elements[11] - _rhs.m_Elements[11],
		m_Elements[12] - _rhs.m_Elements[12], m_Elements[13] - _rhs.m_Elements[13], m_Elements[14] - _rhs.m_Elements[14], m_Elements[15] - _rhs.m_Elements[15]
		);
}

template<typename Real>
Matrix<Real> Matrix<Real>::operator* (Real _scalar) const
{
	return Matrix<Real>(
		_scalar*m_Elements[0], _scalar*m_Elements[1], _scalar*m_Elements[2], _scalar*m_Elements[3],
		_scalar*m_Elements[4], _scalar*m_Elements[5], _scalar*m_Elements[6], _scalar*m_Elements[7],
		_scalar*m_Elements[8], _scalar*m_Elements[9], _scalar*m_Elements[10], _scalar*m_Elements[11],
		_scalar*m_Elements[12], _scalar*m_Elements[13], _scalar*m_Elements[14], _scalar*m_Elements[15]
		);
}

template<typename Real>
Matrix<Real> Matrix<Real>::operator/ (Real _scalar) const
{
	Real InvScalar = ((Real)1.0) / _scalar;
	return Matrix<Real>(
		InvScalar*m_Elements[0], InvScalar*m_Elements[1], InvScalar*m_Elements[2], InvScalar*m_Elements[3],
		InvScalar*m_Elements[4], InvScalar*m_Elements[5], InvScalar*m_Elements[6], InvScalar*m_Elements[7],
		InvScalar*m_Elements[8], InvScalar*m_Elements[9], InvScalar*m_Elements[10],InvScalar*m_Elements[11],
		InvScalar*m_Elements[12], InvScalar*m_Elements[13], InvScalar*m_Elements[14],InvScalar*m_Elements[15]
		);
}

template<typename Real>
Matrix<Real> Matrix<Real>::operator- () const
{
	return Matrix<Real>(-m_Elements[0], -m_Elements[1], -m_Elements[2], -m_Elements[3],
						-m_Elements[4], -m_Elements[5], -m_Elements[6], -m_Elements[7],
						-m_Elements[8], -m_Elements[9], -m_Elements[10], -m_Elements[11],
						-m_Elements[12], -m_Elements[13], -m_Elements[14], -m_Elements[15]);
}

template<typename Real>
Matrix<Real>& Matrix<Real>::operator+= (const Matrix<Real>& _rhs)
{
	m_Elements[0] += _rhs.m_Elements[0]; m_Elements[1] += _rhs.m_Elements[1]; m_Elements[2] += _rhs.m_Elements[2]; m_Elements[3] += _rhs.m_Elements[3];
	m_Elements[4] += _rhs.m_Elements[4]; m_Elements[5] += _rhs.m_Elements[5]; m_Elements[6] += _rhs.m_Elements[6]; m_Elements[7] += _rhs.m_Elements[7];
	m_Elements[8] += _rhs.m_Elements[8]; m_Elements[9] += _rhs.m_Elements[9]; m_Elements[10] += _rhs.m_Elements[10]; m_Elements[11] += _rhs.m_Elements[11];
	m_Elements[12] += _rhs.m_Elements[12]; m_Elements[13] += _rhs.m_Elements[13]; m_Elements[14] += _rhs.m_Elements[14]; m_Elements[15] += _rhs.m_Elements[15];
	
	return *this;
}

template<typename Real>
Matrix<Real>& Matrix<Real>::operator-= (const Matrix<Real>& _rhs)
{
	m_Elements[0] -= _rhs.m_Elements[0]; m_Elements[1] -= _rhs.m_Elements[1]; m_Elements[2] -= _rhs.m_Elements[2]; m_Elements[3] -= _rhs.m_Elements[3];
	m_Elements[4] -= _rhs.m_Elements[4]; m_Elements[5] -= _rhs.m_Elements[5]; m_Elements[6] -= _rhs.m_Elements[6]; m_Elements[7] -= _rhs.m_Elements[7];
	m_Elements[8] -= _rhs.m_Elements[8]; m_Elements[9] -= _rhs.m_Elements[9]; m_Elements[10] -= _rhs.m_Elements[10]; m_Elements[11] -= _rhs.m_Elements[11];
	m_Elements[12] -= _rhs.m_Elements[12]; m_Elements[13] -= _rhs.m_Elements[13]; m_Elements[14] -= _rhs.m_Elements[14]; m_Elements[15] -= _rhs.m_Elements[15];
	
	return *this;
}

template<typename Real>
Matrix<Real>& Matrix<Real>::operator*= (Real _scalar)
{
	m_Elements[0] *= _scalar; m_Elements[1] *= _scalar; m_Elements[2] *= _scalar; m_Elements[3] *= _scalar;
	m_Elements[4] *= _scalar; m_Elements[5] *= _scalar; m_Elements[6] *= _scalar; m_Elements[7] *= _scalar;
	m_Elements[8] *= _scalar; m_Elements[9] *= _scalar; m_Elements[10] *= _scalar; m_Elements[11] *= _scalar;
	m_Elements[12] *= _scalar; m_Elements[13] *= _scalar; m_Elements[14] *= _scalar; m_Elements[15] *= _scalar;
	return *this;
}

template<typename Real>
Matrix<Real>& Matrix<Real>::operator/= (Real _scalar)
{
	Real InvScalar = ((Real)1.0) / _scalar;
	m_Elements[0] *= InvScalar; m_Elements[1] *= InvScalar; m_Elements[2] *= InvScalar; m_Elements[3] *= InvScalar;
	m_Elements[4] *= InvScalar; m_Elements[5] *= InvScalar; m_Elements[6] *= InvScalar; m_Elements[7] *= InvScalar;
	m_Elements[8] *= InvScalar; m_Elements[9] *= InvScalar; m_Elements[10] *= InvScalar; m_Elements[11] *= InvScalar;
	m_Elements[12] *= InvScalar; m_Elements[13] *= InvScalar; m_Elements[14] *= InvScalar; m_Elements[15] *= InvScalar;

	return *this;
}

template<typename Real>
Matrix<Real>& Matrix<Real>::Identity()
{
	static Matrix<Real> out(
		Real(1), Real(0), Real(0), Real(0),
		Real(0), Real(1), Real(0), Real(0), 
		Real(0), Real(0), Real(1), Real(0),
		Real(0), Real(0), Real(0), Real(1)
		);

	return out;
}

using Matrix44 = Matrix<float>;
}