#pragma once

#include "Scene.h"
#include "Input.h"
#include "addit/MyMath.h"
#include "Pipeline.h"
#include "Cube.h"
#include "SolidColorEffect.h"
#include <iostream>

class CubeSolidColorScene : public Scene
{
public:
	typedef Pipeline<SolidColorEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;

public:
	CubeSolidColorScene(Graphics& gfx)
		:incube(Cube::GetSkinned<Vertex>()),
		pipeline(gfx)
	{
		for (auto &vertex : incube.vertices)
		{
			vertex.color = Colors::Blue;
		}
	}

	~CubeSolidColorScene() = default;

	void Reset() override
	{
		theta_x = 0.0f;
		theta_y = 0.0f;
		theta_z = 0.0f;
		offSetZ = 2.0f;
	}

	void Update() override
	{
		const float dt = 1.0f / 60.0f;

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

		const Vec3 trans = { 0.0f, 0.0f, offSetZ };

		pipeline.effect.vs.BindRotation(rot);
		pipeline.effect.vs.BindTranslation(trans);

		pipeline.Draw(incube);
	}

private:
	IndexedTriangleList<Vertex> incube;
	Pipeline pipeline;

	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	float offSetZ = 2.0f;
};