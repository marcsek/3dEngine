#pragma once

#include "../Color.h"
#include "../vendor/Vec3.h"
#include "../vendor/Mat.h"
#include "GeometryShader/DefaultGeometryShader.h"
#include "VertexShader/DefaultVertexShader.h"
#include <algorithm>

class GouraudLightEffect
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
			pos(pos),
			n(src.n)
		{}
		Vertex(const Vec3& pos, const Vec3& n)
			:
			pos(pos),
			n(n)
		{}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
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
		Vec3 n;
	};

	class VertexShader
	{
	public:
		class Output
		{
		public:
			Output() = default;
			Output(const Vec3& pos)
				:
				pos(pos)
			{}
			Output(const Vec3& pos, const Output& src)
				:
				pos(pos),
				color(src.color)
			{}
			Output(const Vec3& pos, const Vec3& color)
				:
				pos(pos),
				color(color)
			{}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				color += rhs.color;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				color -= rhs.color;
				return *this;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(float rhs)
			{
				pos *= rhs;
				color *= rhs;
				return *this;
			}
			Output operator*(float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(float rhs)
			{
				pos /= rhs;
				color /= rhs;
				return *this;
			}
			Output operator/(float rhs) const
			{
				return Output(*this) /= rhs;
			}
		public:
			Vec3 pos;
			Vec3 color;
		};
	public:
		void BindRotation(const f_tdMat3& rotation_in)
		{
			rotation = rotation_in;
		}
		void BindTranslation(const Vec3& translation_in)
		{
			translation = translation_in;
		}

		Output operator()(const Vertex& v) const
		{
			//const auto n = ((in1.pos - in0.pos) % (in2.pos - in0.pos)).GetNormalized();
			const auto d = diffuse * (std::max)(0.0f, -(v.n * rotation) * dir);
			const Vec3 c = color.GetHadamard(d + ambient).Saturate() * 255.0f;

			return { v.pos * rotation + translation, c };
		}

		void SetDiffuseLight(const Vec3& c)
		{
			diffuse = { c.x, c.y, c.z };
		}

		void SetAmbientLight(const Vec3& c)
		{
			ambient = { c.x, c.y, c.z };
		}

		void SetLightDirection(const Vec3& dl)
		{
			assert(dl.LenSq() >= 0.001f);
			dir = dl.GetNormalized();
		}

		void SetMaterialColor(Color c)
		{
			color = Color::GetVector(c.hexValue).GetNormalized();
		}

	private:
		f_tdMat3 rotation = f_tdMat3::Identity();
		Vec3 translation = { 0.0f,  0.0f,  0.0f };

		Vec3 dir = { 0.0f,  0.0f,  1.0f };
		Vec3 diffuse = { 1.0f,  1.0f,  1.0f };
		Vec3 ambient = { 0.1f,  0.1f,  0.1f };
		Vec3 color = { 0.8f,  0.85f, 1.0f };
	};

	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

	class PixelShader
	{
	public:
		template<class InputVer>
		unsigned int operator()(const InputVer& in) const
		{
			return Color::GetHexVec3(in.color);
		}
	};
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};