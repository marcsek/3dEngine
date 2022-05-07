#pragma once
#include "../../Surface.h"

class DefaultPhongPixelShader
{
public:
	template<class Input>
	unsigned int Shade(const Input& in, Vec3 material_color = { 0.8f,0.85f,1.0f }) const
	{
		const auto surf_norm = in.n.GetNormalized();

		auto v_to_l = light_pos - in.worldPos;
		const auto dist = v_to_l.Len();
		const auto dir = v_to_l / dist;


		const auto attenuation = 1.0f /
			(constant_attenuation + linear_attenuation * dist + quadradic_attenuation * MyMath::pow(dist));
		const auto d = light_diffuse * attenuation * (std::max)(0.0f, surf_norm * dir);

		//specular - zalezi na rotacii a pozicii
		v_to_l = light_pos - in.camPos;
		const auto w = surf_norm * (v_to_l * surf_norm);
		const auto r = w * 2.0f - v_to_l;
		const auto s = light_diffuse * specular_intensity * std::pow((std::max)(0.0f, -r.GetNormalized() * in.worldPos.GetNormalized()), specular_power);
		return Color::GetHexVec3(material_color.GetHadamard(d + light_ambient /*+ s*/).Saturate() * 255.0f);
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
	Vec3 light_pos = { 0.0f,0.0f,0.5f };
	Vec3 light_diffuse = { 10.0f,10.0f,10.0f };
	Vec3 light_ambient = { 0.35f,0.35f,0.35f };
	static constexpr float linear_attenuation = 5.0f;
	static constexpr float quadradic_attenuation = 2.619f;
	static constexpr float constant_attenuation = 0.382f;
	static constexpr float specular_power = 30.0f;
	static constexpr float specular_intensity = 0.6f;

};