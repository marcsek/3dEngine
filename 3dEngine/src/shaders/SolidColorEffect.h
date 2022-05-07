#pragma once
#include "../Color.h"
#include "GeometryShader/DefaultGeometryShader.h"

class SolidColorEffect
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
		Vertex(const Vec3& pos, const Color& color)
			:
			color(color),
			pos(pos)
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
		Color color;
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
				pos(pos),
				color(src.color)
			{}
			Output(const Vec4& pos, const Color& color)
				:
				pos(pos),
				color(color)
			{}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				return *this;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(float rhs)
			{
				pos *= rhs;
				return *this;
			}
			Output operator*(float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(float rhs)
			{
				pos /= rhs;
				return *this;
			}
			Output operator/(float rhs) const
			{
				return Output(*this) /= rhs;
			}
		public:
			Vec4 pos;
			Color color;
		};
		void BindWorldView(const f_tdMat4& transformation_in)
		{
			world_view = transformation_in;
			world_view_proj = world_view * proj;
		}

		void BindProjection(const f_tdMat4& transformation_in)
		{
			proj = transformation_in;
			world_view_proj = world_view * proj;
		}

		const f_tdMat4& GetProj() const
		{
			return proj;
		}

		Output operator()(const Vertex& v) const
		{
			return{ Vec4(v.pos) * world_view_proj,v.color };
		}

	private:
		f_tdMat4 world_view = f_tdMat4::Identity();
		f_tdMat4 proj = f_tdMat4::Identity();
		f_tdMat4 world_view_proj = f_tdMat4::Identity();
	};
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

	class PixelShader
	{
	public:
		template<class InputVer>
		unsigned int operator()(const InputVer& in) const
		{
			return in.color.hexValue;
		}
	};
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};