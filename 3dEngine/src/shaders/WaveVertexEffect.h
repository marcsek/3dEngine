#pragma once
#include "../Color.h"
#include "../vendor/Vec3.h"
#include "GeometryShader/DefaultGeometryShader.h"

class WaveVertexEffect
{
public:
	class  Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3& pos)
			:
			pos(pos)
		{}
		Vertex(const Vec3& pos, const Vertex& src)
			:
			color(src.color),
			pos(pos)
		{}
		Vertex(const Vec3& pos, const Vec3& color)
			:
			color(color),
			pos(pos)
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
			color *= rhs;
			return *this;
		}
		Vertex operator*(float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(float rhs)
		{
			pos /= rhs;
			color /= rhs;
			return *this;
		}
		Vertex operator/(float rhs) const
		{
			return Vertex(*this) /= rhs;
		}
	public:
		Vec3 pos;
		Vec3 color;
	};

	class VertexShader
	{
	public:
		typedef Vertex Output;
	public:
		void BindRotation(const f_tdMat3& rotation_in)
		{
			rotation = rotation_in;
		}
		void BindTranslation(const Vec3& translation_in)
		{
			translation = translation_in;
		}
		Output operator()(const Vertex& in) const
		{
			Vec3 pos = in.pos * rotation + translation;
			pos.y += amplitude * std::sin(time * freqScroll + pos.x * freqWave);
			return{ pos, in.color };
		}
		void SetTime(float t)
		{
			time = t;
		}

	private:
		f_tdMat3 rotation;
		Vec3 translation;
		float time = 0.0f;
		float freqWave = 10.0f;
		float freqScroll = 5.0f;
		float amplitude = 0.05f;
	};

	typedef DefaultGeometryShader<Vertex> GeometryShader;

	class PixelShader
	{
	public:
		template<class InputVer>
		unsigned int operator()(const InputVer& in) const
		{
			return Color::GetHex(in);
		}
	};
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};