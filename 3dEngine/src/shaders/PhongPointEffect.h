#pragma once

#include "../Color.h"
#include "../vendor/Vec4.h"
#include "../vendor/Vec3.h"
//#include "../vendor/Mat3.h"
#include "../vendor/Mat.h"
#include "GeometryShader/DefaultGeometryShader.h"
#include "PixelShader/DefaultPhongPixelShader.h"
#include "VertexShader/DefaultVertexShader.h"
#include <algorithm>
#include <iostream>

class PhongPointEffect
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
				n(src.n),
				worldPos(src.worldPos)
			{}
			Output(const Vec4& pos, const Vec3& n, const Vec3& worldPos, const Vec3& camPos)
				:
				pos(pos),
				n(n),
				worldPos(worldPos),
				camPos(camPos)
			{}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
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

		Output operator()(const Vertex& v) const
		{
			const auto p4 = Vec4(v.pos);
			return { p4 * world_view_proj, Vec4{v.n, 0.0f} * world /* view*/, p4 * world, p4 * world * view};
		}

	private:
		f_tdMat4 proj = f_tdMat4::Identity();
		f_tdMat4 view = f_tdMat4::Identity();
		f_tdMat4 world = f_tdMat4::Identity();
		f_tdMat4 world_view_proj = f_tdMat4::Identity();
	};

	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;
	typedef DefaultPhongPixelShader PixelShaderDef;

	class PixelShader : public DefaultPhongPixelShader
	{
	public:
		template<class InputVer>
		unsigned int operator()(const InputVer& in) const
		{
			return Shade(in);
		}
	};

	//class PixelShader
	//{
	//public:
	//	template<class InputVer>
	//	unsigned int operator()(const InputVer& in) const
	//	{
	//		//const auto v_to_l = (light_pos) - in.worldPos;

	//		//const auto dist = v_to_l.Len();

	//		//auto dir = v_to_l / dist;


	//		//const auto attenuation = 1.0f /
	//		//	(constant_attenuation + linear_attenuation * dist + quadratic_attenuation * MyMath::pow(dist));
	//
	//		//const auto d = light_diffuse * attenuation * (std::max)(0.0f, (in.n.GetNormalized()) * dir);

	//		//const auto c = material_color.GetHadamard(d + light_ambient).Saturate() * 255.0f;

	//		//return Color::GetHexVec3(c);


	//		//phong - nezalezi na rotacii a pozicii
	//		const auto surf_norm = in.n.GetNormalized();

	//		auto v_to_l = light_pos - in.worldPos;
	//		const auto dist = v_to_l.Len();
	//		const auto dir = v_to_l / dist;


	//		const auto attenuation = 1.0f /
	//			(constant_attenuation + linear_attenuation * dist + quadradic_attenuation * MyMath::pow(dist));
	//		////// calculate intensity based on angle of incidence and attenuation
	//		const auto d = light_diffuse * attenuation * (std::max)(0.0f, surf_norm * dir);

	//		//specular - zalezi na rotacii a pozicii
	//		v_to_l = light_pos - in.camPos;
	//		const auto w = surf_norm * (v_to_l * surf_norm);
	//		const auto r = w * 2.0f - v_to_l;
	//		const auto s = light_diffuse * specular_intensity * std::pow((std::max)(0.0f, -r.GetNormalized() * in.worldPos.GetNormalized()), specular_power);
	//		return Color::GetHexVec3(material_color.GetHadamard(d + light_ambient /*+ s*/).Saturate() * 255.0f);
	//	}
	//	void SetDiffuseLight(const Vec3& c)
	//	{
	//		light_diffuse = c;
	//	}

	//	void SetAmbientLight(const Vec3& c)
	//	{
	//		light_ambient = c;
	//	}

	//	void SetLightPosition(const Vec3& dl)
	//	{
	//		light_pos = dl;
	//	}
	//private:
	//	//Vec3 light_pos = { 0.0f,  0.0f,  0.5f };
	//	//Vec3 light_diffuse = { 1.0f,  1.0f,  1.0f };
	//	//Vec3 light_ambient = { 0.1f,  0.1f,  0.1f };
	//	//Vec3 material_color = { 0.8f,  0.85f, 1.0f };
	//	//float linear_attenuation = 1.0f;
	//	//float quadratic_attenuation = 2.619f;
	//	//float constant_attenuation = 0.382f;

	///*	template<class Input>
	//	unsigned int Shade(const Input& in, const Vec3& material_color) const
	//	{

	//		const auto surf_norm = in.n.GetNormalized();

	//		const auto v_to_l = light_pos - in.worldPos;
	//		const auto dist = v_to_l.Len();
	//		const auto dir = v_to_l / dist;
	//
	//		const auto attenuation = 1.0f /
	//			(constant_attenuation + linear_attenuation * dist + quadradic_attenuation * sq(dist));

	//		const auto d = light_diffuse * attenuation * (std::max)(0.0f, surf_norm * dir);

	//		const auto w = surf_norm * (v_to_l * surf_norm);
	//		const auto r = w * 2.0f - v_to_l;

	//		const auto s = light_diffuse * specular_intensity * std::pow((std::max)(0.0f, -r.GetNormalized() * in.worldPos.GetNormalized()), specular_power);

	//		return Color(material_color.GetHadamard(d + light_ambient + s).Saturate() * 255.0f);
	//	}*/
	///*	unsigned int operator()(const Input& in) const
	//	{*/
	//		//// re-normalize interpolated surface normal
	//		//const auto surf_norm = in.n.GetNormalized();
	//		//// vertex to light data
	//		//const auto v_to_l = light_pos - in.worldPos;
	//		//const auto dist = v_to_l.Len();
	//		//const auto dir = v_to_l / dist;
	//		//// calculate attenuation
	//		//const auto attenuation = 1.0f /
	//		//	(constant_attenuation + linear_attenuation * dist + quadradic_attenuation * sqrt(dist));
	//		//// calculate intensity based on angle of incidence and attenuation
	//		//const auto d = light_diffuse * attenuation * (std::max)(0.0f, surf_norm * dir);
	//		//// reflected light vector
	//		//const auto w = surf_norm * (v_to_l * surf_norm);
	//		//const auto r = w * 2.0f - v_to_l;
	//		//// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
	//		//const auto s = light_diffuse * specular_intensity * std::pow((std::max)(0.0f, -r.GetNormalized() * in.worldPos.GetNormalized()), specular_power);
	//		//// add diffuse+ambient, filter by material color, saturate and scale
	//		//return Color(material_color.GetHadamard(d + light_ambient + s).Saturate() * 255.0f);
	//	//}

	//	/*void SetDiffuseLight(const Vec3& c)
	//	{
	//		light_diffuse = c;
	//	}
	//	void SetAmbientLight(const Vec3& c)
	//	{
	//		light_ambient = c;
	//	}
	//	void SetLightPosition(const Vec3& pos_in)
	//	{
	//		light_pos = pos_in;
	//	}*/
	//private:
	//	Vec3 material_color = { 0.8f,0.85f,1.0f };
	//	Vec3 light_pos = { 0.0f,0.0f,0.5f };
	//	Vec3 light_diffuse = { 2.0f,2.0f,2.0f };
	//	Vec3 light_ambient = { 0.1f,0.1f,0.1f };
	//	static constexpr float linear_attenuation = 3.0f;
	//	static constexpr float quadradic_attenuation = 2.619f;
	//	static constexpr float constant_attenuation = 0.382f;
	//	static constexpr float specular_power = 30.0f;
	//	static constexpr float specular_intensity = 0.6f;
	//};
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};