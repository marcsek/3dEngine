#pragma once

#include "../Color.h"
#include "../vendor/Vec4.h"
#include "../vendor/Vec3.h"
//#include "../vendor/Mat3.h"
#include "../vendor/Mat.h"
#include "GeometryShader/DefaultGeometryShader.h"
#include "VertexShader/DefaultVertexShader.h"
#include <algorithm>
#include <iostream>

#include "../Surface.h"

class Surface;

class LightRippleTextureEffect
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
			t(src.t)
		{}
		Vertex(const Vec3& pos, const Vec3& t)
			:
			pos(pos),
			t(t)
		{}
	public:
		Vec3 pos;
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
				n(src.n),
				pos(pos),
				worldPos(src.worldPos),
				camPos(src.camPos)
			{}
			Output(const Vec4& pos, const Vec2& t, const Vec3& n, const Vec3& worldPos, const Vec3& camPos)
				:
				t(t),
				pos(pos),
				n(n),
				worldPos(worldPos),
				camPos(camPos)
			{}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				t += rhs.t;
				n += rhs.n;
				worldPos += rhs.worldPos;
				camPos += rhs.camPos;
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
				n -= rhs.n;
				worldPos -= rhs.worldPos;
				camPos -= rhs.camPos;
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
				n *= rhs;
				worldPos *= rhs;
				camPos *= rhs;
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
				n /= rhs;
				worldPos /= rhs;
				camPos /= rhs;
				return *this;
			}
			Output operator/(float rhs) const
			{
				return Output(*this) /= rhs;
			}
		public:
			Vec4 pos;
			Vec2 t;
			Vec3 n;
			Vec3 worldPos;
			Vec3 camPos;
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

		void SetTime(float t_in)
		{
			t = t_in;
		}

		template<class InputVer>
		Output operator()(const InputVer& in) const
		{
			const auto angle = MyMath::WrapAngle(in.pos.x * freq + (t / 10) * wavelength);
			const auto cosx = std::cos(angle);
			const auto sinx = std::sin(angle);

			const auto dz = amplitude * cosx;
			const auto pos = Vec4{ in.pos.x, in.pos.y, in.pos.z + dz, 1.0f };

			auto n = Vec4{
				-freq * amplitude * sinx,
				0.0f,
				-1.0f,
				0.0f
			};
			n.Normalize();
			return { pos * world_view_proj,in.t, n * world * view, pos * world, pos * world * view };
		}

	private:
		static constexpr float wavelength = MyMath::PI;
		static constexpr float freq = 27.5f;
		static constexpr float amplitude = 0.02f;
		float t = 0.0f;

		f_tdMat4 proj = f_tdMat4::Identity();
		f_tdMat4 view = f_tdMat4::Identity();
		f_tdMat4 world = f_tdMat4::Identity();
		f_tdMat4 world_view_proj = f_tdMat4::Identity();

	};
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

	class PixelShader : public DefaultPhongPixelShader
	{
	public:
		template<class InputVer>
		unsigned int operator()(const InputVer& in) const
		{
			unsigned int x = static_cast<unsigned int>(in.t.x * t_width_ + 0.5f) % t_width_;
			unsigned int y = static_cast<unsigned int>(in.t.y * t_height_ + 0.5f) % t_height_;

			Color color = pixel_texture_->get_pixel(x, y);

			return Shade(in, color.rgb / 255.0f);
		}
		void bind_texture(const Surface* tex)
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
