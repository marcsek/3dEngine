#pragma once
#include "vendor/Vec3.h"

class  Vertex
{
public:
	Vertex() = default;
	Vertex(const Vec3& pos)
		:pos(pos)
	{}
	Vertex(const Vec3& pos, unsigned int color)
		:pos(pos), color(color)
	{}

	Vertex& operator+=(const Vertex& rhs)
	{
		pos += rhs.pos;
		color += rhs.color;
		return *this;
	}
	Vertex operator+(const Vertex& rhs) const
	{
		return Vertex(*this) += rhs;
	}
	Vertex& operator-=(const Vertex& rhs)
	{
		pos -= rhs.pos;
		color -= rhs.color;
		return *this;
	}
	Vertex operator-(const Vertex& rhs) const
	{
		return Vertex(*this) -= rhs;
	}
	Vertex& operator*=(float rhs)
	{
		pos *= rhs;
		return *this;
	}
	Vertex operator*(float rhs) const
	{
		return Vertex(*this) *= rhs;
	}
	Vertex& operator/=(float rhs)
	{
		pos /= rhs;
		return *this;
	}
	Vertex operator/(float rhs) const
	{
		return Vertex(*this) /= rhs;
	}

public:
	Vec3 pos;
	unsigned int color = 0xFFFFF;
};