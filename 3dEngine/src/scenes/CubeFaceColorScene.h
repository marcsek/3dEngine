#pragma once

#include "Scene.h"
#include "Input.h"
#include "addit/MyMath.h"
#include "Pipeline.h"
#include "Cube.h"
#include "SolidColorEffect.h"
#include <iostream>

class CubeFaceColorScene : public Scene
{
public:
	typedef Pipeline<SolidColorEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;

public:
	CubeFaceColorScene(Graphics& gfx)
		:incube(Cube::GetSkinnedIndependent<Vertex>()),
		pipeline(gfx)
	{
		std::vector<Color> colors
		{
			Colors::Blue,
			Colors::Cyan,
			Colors::Green,
			Colors::Magenta,
			Colors::Red,
			Colors::Silver,
		};

		for (size_t i = 0; i < incube.vertices.size(); i++)
		{
			incube.vertices[i].color = colors[i / 4];
		}
	}

	~CubeFaceColorScene() = default;

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
		const f_tdMat3 rot2 =
			f_tdMat3::RotationX(-theta_x)
			*
			f_tdMat3::RotationY(-theta_y)
			*
			f_tdMat3::RotationZ(-theta_y);

		const Vec3 trans2 = { 0.0f, 0.0f, offSetZ };

		pipeline.effect.vs.BindRotation(rot2);
		pipeline.effect.vs.BindTranslation(trans2);

		pipeline.Draw(incube);

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