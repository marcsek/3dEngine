#pragma once

#include "Scene.h"
#include "Input.h"
#include "addit/MyMath.h"
#include "Pipeline.h"
#include "Plane.h"
#include "Cube.h"
#include "LightFlatEffect.h"
#include "GouraudLightEffect.h"

class ObjModelLighScene : public Scene
{
public:
	typedef Pipeline<GouraudLightEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;

public:
	ObjModelLighScene(Graphics& gfx, IndexedTriangleList<Vertex> inmodel)
		:incube(std::move(inmodel)),
		pipeline(gfx)
	{
		incube.AdjustToTrueCenter();
		offSetZ = incube.GetRadius() * 1.6f;
	}

	~ObjModelLighScene() = default;

	void Reset() override
	{
		theta_x = 0.0f;
		theta_y = 0.0f;
		theta_z = 0.0f;
		offSetZ = incube.GetRadius() * 1.6f;
		//pipeline.effect.vs.SetLightDirection({ 0.0f, 0.0f, 1.0f });
		phi_y = 0.0f;
	}

	void Update() override
	{
		const float dt = 1.0f / 60.0f;
		float t = 0.0f;

		if (Input::CheckInput('Q'))
		{
			theta_x = MyMath::WrapAngle(theta_x + MyMath::PI * dt);
		}
		if (Input::CheckInput('W'))
		{
			theta_y = MyMath::WrapAngle(theta_y + MyMath::PI * dt);
		}
		if (Input::CheckInput('E'))
		{
			theta_z = MyMath::WrapAngle(theta_z + MyMath::PI * dt);
		}
		if (Input::CheckInput('A'))
		{
			theta_x = MyMath::WrapAngle(theta_x - MyMath::PI * dt);
		}
		if (Input::CheckInput('S'))
		{
			theta_y = MyMath::WrapAngle(theta_y - MyMath::PI * dt);
		}
		if (Input::CheckInput('D'))
		{
			theta_z = MyMath::WrapAngle(theta_z - MyMath::PI * dt);
		}
		if (Input::CheckInput('Z'))
		{
			offSetZ += offSetZ * dt;
		}
		if (Input::CheckInput('X'))
		{
			offSetZ -= offSetZ * dt;
		}
		if (Input::CheckInput('C'))
		{
			phi_y = MyMath::WrapAngle(phi_y + MyMath::PI * std::sin(dt));
		}
		if (Input::CheckInput('V'))
		{
			phi_y = MyMath::WrapAngle(phi_y - MyMath::PI * std::sin(dt));
		}
	}

	void Draw() override
	{
		pipeline.BeginFrame();
		const f_tdMat3 rot =
			f_tdMat3::RotationX(theta_x)
			*
			f_tdMat3::RotationY(theta_y)
			*
			f_tdMat3::RotationZ(theta_z);

		const f_tdMat3 rot_phi =
			f_tdMat3::RotationX(phi_x) *
			f_tdMat3::RotationY(phi_y) *
			f_tdMat3::RotationZ(phi_z);

		const Vec3 trans = { 0.0f, 0.0f, offSetZ };

		pipeline.effect.vs.BindRotation(rot);
		pipeline.effect.vs.BindTranslation(trans);
		//pipeline.effect.vs.SetLightDirection(light_dir * rot_phi);

		pipeline.Draw(incube);
	}

private:
	IndexedTriangleList<Vertex> incube;
	Pipeline pipeline;

	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	float offSetZ = 2.0f;
	float phi_x = 0.0f;
	float phi_y = 0.0f;
	float phi_z = 0.0f;
	Vec3 light_dir = { 0.2f,-0.5f,1.0f };
};