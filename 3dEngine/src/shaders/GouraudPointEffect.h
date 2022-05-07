#pragma once

#include "../Color.h"
#include "../vendor/Vec3.h"
#include "GeometryShader/DefaultGeometryShader.h"
#include "VertexShader/DefaultVertexShader.h"
#include <algorithm>
#include <iostream>

class GouraudPointEffect
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
			// transform mech vertex position before lighting calc
			const auto pos = v.pos * rotation + translation;
			// vertex to light data
			const auto v_to_l = (light_pos * 2) - pos;
			const auto dist = v_to_l.Len();
			const auto dir = v_to_l / dist;
			// calculate attenuation
			const auto attenuation = 1.0f /
				(constant_attenuation + linear_attenuation * dist + quadratic_attenuation * sqrt(dist));
			// calculate intensity based on angle of incidence and attenuation
			const auto d = light_diffuse * attenuation * (std::max)(0.0f, (v.n * rotation) * dir);
			// add diffuse+ambient, filter by material color, saturate and scale
			const auto c = material_color.GetHadamard(d + light_ambient).Saturate() * 255.0f;
			return{ pos,c };
		}

		void SetDiffuseLight(const Vec3& c)
		{
			light_diffuse = c;
		}

		void SetAmbientLight(const Vec3& c)
		{
			light_ambient = c;
		}

		void SetLightPosition(const Vec3& dl)
		{
			light_pos = dl;
		}

	private:
		f_tdMat3 rotation = f_tdMat3::Identity();
		Vec3 translation = { 0.0f,  0.0f,  0.0f };

		Vec3 light_pos = { 0.0f,  0.0f,  0.5f };
		Vec3 light_diffuse = { 1.0f,  1.0f,  1.0f };
		Vec3 light_ambient = { 0.1f,  0.1f,  0.1f };
		Vec3 material_color = { 0.8f,  0.85f, 1.0f };
		float linear_attenuation = 1.0f;
		float quadratic_attenuation = 2.619f;
		float constant_attenuation = 0.382f;
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