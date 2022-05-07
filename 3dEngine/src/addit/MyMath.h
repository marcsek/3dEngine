#pragma once

#include <cmath>
#include "../vendor/Vec2.h"

namespace MyMath {

	static constexpr float PI = 3.14159265f;
	static constexpr double PI_D = 3.1415926535897932;

	template<typename T>
	_Vec2<T> RotateVec2(T angle, _Vec2<T> vec_in)
	{
		const T cosTheta = cos(angle);
		const T sinTheta = sin(angle);

		const T newX = vec_in.x * cosTheta - vec_in.y * sinTheta;
		const T newVecY = vec_in.x * sinTheta + vec_in.y * cosTheta;
		const T newVecX = newX;

		return _Vec2<T>{ newVecX, newVecY };
	}

	//square (x * x)
	template<typename T>
	auto pow(T thing_to_square)
	{
		return thing_to_square * thing_to_square;
	}

	inline int RandomNum(int start, int end)
	{
		return rand() % (end - start) + start;
	}

	template<typename T>
	inline T WrapAngle(T theta)
	{
		const T modded = fmod(theta, (T)2.0 * (T)PI_D);
		return (modded > (T)PI_D) ? (modded - (T)2.0 * (T)PI_D) : modded;
	}

	template<typename T>
	constexpr T interpolate(const T& src, const T& dst, float alpha)
	{
		return src + (dst - src) * alpha;
	}

	template<typename T>
	constexpr T to_rad(T degrees)
	{
		return (degrees * (T)PI) / 180;
	}
}


