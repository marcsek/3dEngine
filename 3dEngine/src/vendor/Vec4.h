#pragma once

#include <algorithm>
#include "Vec3.h"

template <typename T>
class _Vec4 : public _Vec3<T>
{
public:
	_Vec4() = default;
	_Vec4(T x, T y, T z, T w)
		:
		Vec3(x, y, z),
		w(w)
	{}
	_Vec4(const _Vec3<T>& v3, float w = 1.0f)
		:
		Vec3(v3),
		w(w)
	{}
	template <typename T2>
	explicit operator _Vec4<T2>() const
	{
		return{ (T2)Vec3::x,(T2)Vec3::y,(T2)Vec3::z,(T2)w };
	}
	//T		LenSq() const
	//{
	//	return sq( *this );
	//}
	//T		Len() const
	//{
	//	return sqrt( LenSq() );
	//}
	//_Vec3&	Normalize()
	//{
	//	const T length = Len();
	//	x /= length;
	//	y /= length;
	//	z /= length;
	//	return *this;
	//}
	//_Vec3	GetNormalized() const
	//{
	//	_Vec3 norm = *this;
	//	norm.Normalize();
	//	return norm;
	//}
	_Vec4	operator-() const
	{
		return _Vec4(-Vec3::x, -Vec3::y, -Vec3::z, -w);
	}
	_Vec4& operator=(const _Vec4& rhs)
	{
		Vec3::x = rhs.x;
		Vec3::y = rhs.y;
		Vec3::z = rhs.z;
		w = rhs.w;
		return *this;
	}
	_Vec4& operator+=(const _Vec4& rhs)
	{
		Vec3::x += rhs.x;
		Vec3::y += rhs.y;
		Vec3::z += rhs.z;
		w += rhs.w;
		return *this;
	}
	_Vec4& operator-=(const _Vec4& rhs)
	{
		Vec3::x -= rhs.x;
		Vec3::y -= rhs.y;
		Vec3::z -= rhs.z;
		w -= rhs.w;
		return *this;
	}
	//T		operator*( const _Vec4 &rhs ) const
	//{
	//	return x * rhs.x + y * rhs.y + z * rhs.z;
	//}
	_Vec4	operator+(const _Vec4& rhs) const
	{
		return _Vec4(*this) += rhs;
	}
	_Vec4	operator-(const _Vec4& rhs) const
	{
		return _Vec4(*this) -= rhs;
	}
	_Vec4& operator*=(const T& rhs)
	{
		Vec3::x *= rhs;
		Vec3::y *= rhs;
		Vec3::z *= rhs;
		w *= rhs;
		return *this;
	}
	_Vec4	operator*(const T& rhs) const
	{
		return _Vec4(*this) *= rhs;
	}
	//_Vec4	operator%( const _Vec4& rhs ) const
	//{
	//	return _Vec4(
	//		y * rhs.z - z * rhs.y,
	//		z * rhs.x - x * rhs.z,
	//		x * rhs.y - y * rhs.x );
	//}
	_Vec4& operator/=(const T& rhs)
	{
		Vec3::x /= rhs;
		Vec3::y /= rhs;
		Vec3::z /= rhs;
		w /= rhs;
		return *this;
	}
	_Vec4	operator/(const T& rhs) const
	{
		return _Vec4(*this) /= rhs;
	}
	bool	operator==(const _Vec4& rhs) const
	{
		return Vec3::x == rhs.x && Vec3::y == rhs.y && rhs.z == Vec3::z && rhs.w == w;
	}
	bool	operator!=(const _Vec4& rhs) const
	{
		return !(*this == rhs);
	}
	// clamp to between 0.0 ~ 1.0
	_Vec4& Saturate()
	{
		Vec3::x = (std::min)(1.0f, (std::max)(0.0f, Vec3::x));
		Vec3::y = (std::min)(1.0f, (std::max)(0.0f, Vec3::y));
		Vec3::z = (std::min)(1.0f, (std::max)(0.0f, Vec3::z));
		w = (std::min)(1.0f, (std::max)(0.0f, w));
		return *this;
	}
	// clamp to between 0.0 ~ 1.0
	_Vec4	GetSaturated() const
	{
		_Vec4 temp(*this);
		temp.Saturate();
		return temp;
	}
	// x3 = x1 * x2 etc.
	_Vec4& Hadamard(const _Vec4& rhs)
	{
		Vec3::x *= rhs.x;
		Vec3::y *= rhs.y;
		Vec3::z *= rhs.z;
		w *= rhs.w;
		return *this;
	}
	// x3 = x1 * x2 etc.
	_Vec4	GetHadamard(const _Vec4& rhs) const
	{
		_Vec4 temp(*this);
		temp.Hadamard(rhs);
		return temp;
	}
public:
	T w;
};

typedef _Vec4<float> Vec4;
typedef _Vec4<double> Ved4;
typedef _Vec4<int> Vei4;