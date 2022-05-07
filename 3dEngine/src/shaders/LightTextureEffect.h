#pragma once

#include "../Color.h"
#include "../vendor/Vec4.h"
#include "../vendor/Vec3.h"
//#include "../vendor/Mat3.h"
#include "shaders/PixelShader/DefaultPhongPixelShader.h"
#include "../vendor/Mat.h"
#include "GeometryShader/DefaultGeometryShader.h"
#include "VertexShader/DefaultVertexShader.h"
#include <algorithm>
#include <iostream>

#include "../Surface.h"

class Surface;

class LightTextureEffect
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
	public:
		Vec3 pos;
		Vec3 n;
		Vec2 t;
	};

	class VertexShader
	{
	public:
		class Output
		{
		public:
			Output() = default;
			Output(const Vec4& pos)
				:
				pos(pos)
			{}
			Output(const Vec4& pos, const Output& src)
				:
				t(src.t),
				l(src.l),
				pos(pos)
			{}
			Output(const Vec4& pos, const Vec2& t, const Vec3& l)
				:
				t(t),
				pos(pos),
				l(l)
			{}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				t += rhs.t;
				l += rhs.l;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				t -= rhs.t;
				l -= rhs.l;
				return *this;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(float rhs)
			{
				pos *= rhs;
				t *= rhs;
				l *= rhs;
				return *this;
			}
			Output operator*(float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(float rhs)
			{
				pos /= rhs;
				t /= rhs;
				l /= rhs;
				return *this;
			}
			Output operator/(float rhs) const
			{
				return Output(*this) /= rhs;
			}
		public:
			Vec4 pos;
			Vec2 t;
			Vec3 l;
		};
	public:
		void BindView(const f_tdMat4& transformation_in)
		{
			view = transformation_in;
		}

		void BindWorldView(const f_tdMat4& transformation_in)
		{
			world = transformation_in;
			world_view_proj = world * proj;
		}

		void BindProjection(const f_tdMat4& transformation_in)
		{
			proj = transformation_in;
			world_view_proj = world * view * proj;
		}

		const f_tdMat4& GetProj() const
		{
			return proj;
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

		Output operator()(const Vertex& v) const
		{
			//const auto p4 = Vec4(v.pos);
			//return { p4 * world_view_proj, Vec4{v.n, 0.0f} *world /* view*/, p4 * world, p4 * world * view };
			const auto p4 = Vec4(v.pos);
			// transform mech vertex position before lighting calc
			const auto worldPos = p4 * world;
			// vertex to light data
			const auto v_to_l = light_pos - worldPos;
			const auto dist = v_to_l.Len();
			const Vec3 dir = v_to_l / dist;
			// calculate attenuation
			const auto attenuation = 1.0f /
				(constant_attenuation + linear_attenuation * dist * quadradic_attenuation * MyMath::pow(dist));
			// calculate intensity based on angle of incidence and attenuation
			const auto d = light_diffuse * attenuation * (std::max)(0.0f, static_cast<Vec3>(Vec4(v.n, 0.0f) * world /** view*/) * dir);
			// add diffuse+ambient, filter by material color, saturate and scale
			const auto l = d + light_ambient;

			return { p4 * world_view_proj, v.t, l };
		}

	private:
		f_tdMat4 proj = f_tdMat4::Identity();
		f_tdMat4 view = f_tdMat4::Identity();
		f_tdMat4 world = f_tdMat4::Identity();
		f_tdMat4 world_view_proj = f_tdMat4::Identity();

		Vec3 material_color = { 0.8f,0.85f,1.0f };
		Vec3 light_pos = { 0.0f,0.0f,0.5f };
		Vec3 light_diffuse = { 15.0f,15.0f,15.0f };
		Vec3 light_ambient = { 0.35f,0.35f,0.35f };
		static constexpr float linear_attenuation = 1.0f;
		static constexpr float quadradic_attenuation = 2.619f;
		static constexpr float constant_attenuation = 0.382f;
		static constexpr float specular_power = 30.0f;
		static constexpr float specular_intensity = 0.6f;
	};
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;



	class PixelShader
	{
	public:
		template<class InputVer>
		unsigned int operator()(const InputVer& in) const
		{
			unsigned int x = static_cast<unsigned int>(in.t.x * t_width_ + 0.5f) % t_width_;
			unsigned int y = static_cast<unsigned int>(in.t.y * t_height_ + 0.5f) % t_height_;

			Color color = pixel_texture_->get_pixel(x, y);

			const auto material_color = color.rgb / 255.0f;
			return Color(Color::GetHexVec3(material_color.GetHadamard(in.l).Saturate() * 255.0f)).hexValue;
		}
		void bind_texture(const Surface *tex)
		{
			pixel_texture_ = tex;
			t_width_ = tex->get_width();
			t_height_ = tex->get_height();
		}

	private:
		const Surface* pixel_texture_ = nullptr;
		unsigned int t_width_ = 0, t_height_ = 0;
	};
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};
