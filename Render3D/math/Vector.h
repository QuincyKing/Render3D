/*!
* @file Vector.h
* @date 2017/05/13 17:58
*
* @author QuincyKing
* Contact: j88172365@gmail.com
*
* @brief 向量类
*/
#pragma once

#include <cassert>
namespace Math3D
{
	template<typename Real, int size>
	class Vector
	{
	public:
		typedef Real value_type;

	public:
		Vector() { for (int i = 0; i < size; i++) m_Tuple[i] = Real(0); }
		Vector(Real _realX, Real _realY);
		Vector(Real _realX, Real _realY, Real _realZ);
		Vector(Real _realX, Real _realY, Real _realZ, Real _realW);
		Vector(const Real *_realTuple);
		Vector(Real _scalar);
		Vector(const Vector& _rhs);
		Vector& operator = (const Vector& _rhs);
		bool HasNaNs() const;

		const Real* operator() (void)const		{ return m_Tuple; }
		Real* operator() (void)					{ return m_Tuple; }
		Real operator[] (size_t i) const		{ return m_Tuple[i]; }
		Real& operator[] (size_t i)				{ return m_Tuple[i]; }
	    Real X()  const							{ return m_Tuple[0]; }
		Real& X()								{ return m_Tuple[0]; }
		Real Y()  const							{ static_assert(size >= 2, "Vector dimension error."); return m_Tuple[1]; }
		Real& Y()								{ static_assert(size >= 2, "Vector dimension error."); return m_Tuple[1]; }
		Real Z() const							{ static_assert(size >= 3, "Vector dimension error."); return m_Tuple[2]; }
		Real& Z()								{ static_assert(size >= 3, "Vector dimension error."); return m_Tuple[2]; }
		Real W() const							{ static_assert(size == 4, "Vector dimension error."); return m_Tuple[3]; }
		Real& W()								{ static_assert(size == 4, "Vector dimension error."); return m_Tuple[3]; }
		
		Vector operator+ (const Vector& _rhs) const;
		Vector operator- (const Vector& _rhs) const;
		Vector operator* (const Vector& _rhs) const;
		Vector operator* (Real _scalar) const;
		Vector operator/ (Real _scalar) const;
		Vector operator- () const;
		Vector& operator+= (const Vector& _rhs);
		Vector& operator-= (const Vector& _rhs);
		Vector& operator*= (Real _scalar);
		Vector& operator/= (Real _scalar);
		bool operator== (const Vector& _rhs) const;
		bool operator!= (const Vector& _rhs) const;
		static const Vector& Zero();

	private:
		Real m_Tuple[size];
	};

	template<typename Real, int size>
	Vector<Real, size>::Vector(Real _realX, Real _realY)
	{
		static_assert(size == 2, "向量维度错误");

		m_Tuple[0] = _realX;
		m_Tuple[1] = _realY;
	}

	template<typename Real, int size>
	Vector<Real, size>::Vector(Real _realX, Real _realY, Real _realZ)
	{
		assert(size == 3);

		m_Tuple[0] = _realX;
		m_Tuple[1] = _realY;
		m_Tuple[2] = _realZ;
	}

	template<typename Real, int size>
	Vector<Real, size>::Vector(Real _realX, Real _realY, Real _realZ, Real _realW)
	{
		static_assert(size == 4, "向量维度错误");

		m_Tuple[0] = _realX;
		m_Tuple[1] = _realY;
		m_Tuple[2] = _realZ;
		m_Tuple[3] = _realW;
	}

	template<typename Real, int size>
	Vector<Real, size>::Vector(const Real _realTuple[])
	{
		for (int i = 0; i < size; i++)
		{
			m_Tuple[i] = _realTuple[i];
		}
	}

	template<typename Real, int size>
	Vector<Real, size>::Vector(Real _scalar)
	{
		for (int i = 0; i < size; i++)
		{
			m_Tuple[i] = _scalar;
		}
	}

	template<typename Real, int size>
	Vector<Real, size>::Vector(const Vector& _rhs)
	{
		for (int i = 0; i < size; i++)
		{
			m_Tuple[i] = _rhs[i];
		}
	}

	template<typename Real, int size>
	Vector<Real, size>& Vector<Real, size>::operator = (const Vector<Real, size>& _rhs)
	{
		for (int i = 0; i < size; i++)
		{
			m_Tuple[i] = _rhs[i];
		}
		return *this;
	}

	template<typename Real, int size>
	bool Vector<Real, size>::HasNaNs() const
	{
		int anyNaN = 0;
		for (int i = 0; i < size&&!anyNaN; i++)
		{
			anyNaN = isnan(m_Tuple[i]);
		}
		return anyNaN != 0;
	}

	template<typename Real, int size>
	Vector<Real, size> Vector<Real, size>::operator+ (const Vector<Real, size>& _rhs) const
	{
		Vector result;
		for (int i = 0; i < size; i++)
			result[i] = _rhs[i] + m_Tuple[i];
		return result;
	}

	template<typename Real, int size>
	Vector<Real, size> Vector<Real, size>::operator- (const Vector<Real, size>& _rhs) const
	{
		Vector result;
		for (int i = 0; i < size; i++)
			result[i] = m_Tuple[i] - _rhs[i];
		return result;
	}

	template<typename Real, int size>
	Vector<Real, size> Vector<Real, size>::operator* (const Vector<Real, size>& _rhs) const
	{
		Vector result;
		for (int i = 0; i < size; i++)
			result[i] = m_Tuple[i] * _rhs[i];
		return result;
	}

	template<typename Real, int size>
	Vector<Real, size> Vector<Real, size>::operator* (Real _scalar) const
	{
		Vector<Real, size> result;
		for (int i = 0; i < size; i++)
			result[i] = m_Tuple[i] * _scalar;
		return result;
	}

	template<typename Real, int size>
	Vector<Real, size> Vector<Real, size>::operator/ (Real _scalar) const
	{
		Vector<Real, size> result;
		for (int i = 0; i < size; i++)
			result[i] = m_Tuple[i] / _scalar;
		return result;
	}

	template<typename Real, int size>
	Vector<Real, size> Vector<Real, size>::operator- () const
	{
		Vector<Real, size> result;
		for (int i = 0; i < size; i++)
			result[i] = -m_Tuple[i];
		return result;
	}

	template<typename Real, int size>
	Vector<Real, size>& Vector<Real, size>::operator+= (const Vector<Real, size>& _rhs)
	{
		for (int i = 0; i < size; i++)
			m_Tuple[i] += _rhs[i];
		return *this;
	}

	template<typename Real, int size>
	Vector<Real, size>& Vector<Real, size>::operator-= (const Vector<Real, size>& _rhs)
	{
		for (int i = 0; i < size; i++)
			m_Tuple[i] -= _rhs[i];
		return *this;
	}

	template<typename Real, int size>
	Vector<Real, size>& Vector<Real, size>::operator*= (Real _scalar)
	{
		for (int i = 0; i < size; i++)
			m_Tuple[i] *= _scalar;
		return *this;
	}

	template<typename Real, int size>
	Vector<Real, size>& Vector<Real, size>::operator/= (Real _scalar)
	{
		for (int i = 0; i < size; i++)
			m_Tuple[i] /= _scalar;
		return *this;
	}

	template<typename Real, int size>
	bool Vector<Real, size>::operator== (const Vector<Real, size>& _rhs) const
	{
		return memcmp(m_Tuple, _rhs.m_Tuple, size*sizeof(Real)) == 0;
	}

	template<typename Real, int size>
	bool Vector<Real, size>::operator!= (const Vector<Real, size>& _rhs) const
	{
		return memcmp(m_Tuple, _rhs.m_Tuple, size*sizeof(Real)) != 0;
	}

	template<typename Real, int size>
	static const Vector<Real, size>& Vector<Real, size>::Zero()
	{
		static const Vector<Real, size> zero = Vector<Real, size>(Real(0));
		return zero;
	}

	using Vector2 = Vector<float, 2>;
	using Vector3 = Vector<float, 3>;
	using Vector4 = Vector<float, 4>;
}
