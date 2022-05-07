#pragma once

#include "../Scene.h"
#include "../Input.h"
#include "../addit/MyMath.h"
#include "../Pipeline.h"
#include "../Plane.h"
#include "../Sphere.h"
#include "../shaders/SolidColorEffect.h"
#include "../shaders/PhongPointEffect.h"
#include "../vendor/Mat.h"
#include "../Surface.h"
#include "../shaders/LightTextureEffect.h"
#include "../shaders/LightRippleTextureEffect.h"
#include <filesystem>
#include "../Keyboard.h"
#include "MouseTracker.h"
#include <iostream>
#include <thread>

using std::filesystem::current_path;

class GouraudPointScene : public Scene
{
public:
	typedef Pipeline<PhongPointEffect> PhongPointPip;
	typedef ::Pipeline<SolidColorEffect> LightIndicatorPip;
	typedef ::Pipeline<LightTextureEffect> LightTexturePip;
	typedef ::Pipeline<LightRippleTextureEffect> LightRippleTexPip;

public:
	GouraudPointScene(Graphics& gfx)
		:pZb(std::make_shared<ZBuffer>(Graphics::widthR, Graphics::heightR)),
		m_main_model(IndexedTriangleList<PhongPointPip::Vertex>::load(std::filesystem::current_path().string() + R"(\src\models\suzanne.obj)")),
		m_light_model(Sphere::GetPlain<SolidColorEffect::Vertex>(0.05f)),
		m_wall_plain_model(Plane::GetSkinnedNormals<LightTextureEffect::Vertex>(20, 20, 2.0f, 2.0f, 1.0f)),
		m_flag_plain_model(Plane::GetSkinned<LightRippleTexPip::Vertex>(50, 10, 1.2f, 1.2f, 1.2f)),
		phong_point_pip(gfx, pZb),
		light_indicator_pip(gfx, pZb),
		light_texture_pip(gfx, pZb),
		light_ripple_tex_pip(gfx, pZb)
	{
		m_main_model.AdjustToTrueCenter();
		main_mod_pos.z = m_main_model.GetRadius() * 1.6f;
		light_pos.z = main_mod_pos.z - 1.5f;

		for(auto& v :m_light_model.vertices)
		{
			v.color = Colors::White;
		}

		//load walls
		for (int i = 0; i < 2; i++)
		{
			walls.push_back(
				WallObj
				{
				&m_tex_floor,
				Plane::GetSkinnedNormals<LightTextureEffect::Vertex>(20, 20, 5.0f, 5.0f, 0.5f),
				  f_tdMat4::Translation(0.0f , 0.0f, 2.5f) * f_tdMat4::RotationX((MyMath::PI * (0.5f + float(i))))
				}
			);
		}

		for (int i = 0; i < 4; i++)
		{
			walls.push_back(
				WallObj
				{
				&m_tex_wall,
				Plane::GetSkinnedNormals<LightTextureEffect::Vertex>(20, 20, 5.0f, 5.0f, 0.5f),
				 f_tdMat4::Translation(0.0f , 0.0f, 2.5f) * f_tdMat4::RotationY((MyMath::PI / 2.0f) * float(i))
				}
			);
		}
	}

	~GouraudPointScene() override = default;

	void Reset() override
	{
		light_pos.z = main_mod_pos.z - 1.5f;
	}

	void Update(Keyboard& kbd, Mouse& mouse) override
	{
		const float dt = 1.0f / 60.0f;
		game_time++;

		light_pos.y = std::sin(dt * game_time * 2);

		light_ripple_tex_pip.effect.vs.SetTime(game_time);

		if(kbd.KeyIsPressed('Q'))
		{
			m_main_model_theta_x = MyMath::WrapAngle(m_main_model_theta_x + MyMath::PI * dt);
		}

		//if (Input::CheckInput('Q'))
		//{
		//	m_main_model_theta_x = MyMath::WrapAngle(m_main_model_theta_x + MyMath::PI * dt);
		//}
		if (kbd.KeyIsPressed('W'))
		{
			cam_pos += Vec4{0.0f, 0.0f, 1.0f, 0.0f} * !cam_rot *  cam_speed * dt;
		}
		if (kbd.KeyIsPressed('A'))
		{
			cam_pos -= Vec4{ 1.0f, 0.0f, 0.0f, 0.0f } *!cam_rot * cam_speed * dt;
		}
		if (kbd.KeyIsPressed('S'))
		{
			cam_pos -= Vec4{ 0.0f, 0.0f, 1.0f, 0.0f } *!cam_rot * cam_speed * dt;
		}
		if (kbd.KeyIsPressed('D'))
		{
			cam_pos += Vec4{ 1.0f, 0.0f, 0.0f, 0.0f } *!cam_rot * cam_speed * dt;
		}
		if(kbd.KeyIsPressed('I'))
		{
			m_camera_theta_y += 500.0f * dt;
		}
		if (kbd.KeyIsPressed('J'))
		{
			m_camera_theta_x += 500.0f * dt;
		}
		if (kbd.KeyIsPressed('K'))
		{
			m_camera_theta_y -= 500.0f * dt;
		}
		if (kbd.KeyIsPressed('L'))
		{
			m_camera_theta_x -= 500.0f * dt;
		}
		if (kbd.KeyIsPressed('X'))
		{
			main_mod_pos.z += main_mod_pos.z * dt;
		}
		if (kbd.KeyIsPressed('T'))
		{
			light_pos.x += 0.5f * dt;
		}
		if (kbd.KeyIsPressed('F'))
		{
			light_pos.y -= 0.5f * dt;
		}
		if (kbd.KeyIsPressed('G'))
		{
			light_pos.x -= 0.5f * dt;
		}
		if (kbd.KeyIsPressed('H'))
		{
			light_pos.y += 0.5f * dt;
		}
		if (kbd.KeyIsPressed('U'))
		{
			light_pos.z -= 0.5f * dt;
		}
		if (kbd.KeyIsPressed('O'))
		{
			light_pos.z += 0.5f * dt;
		}

		const auto pair = mouse.GetPos();
		if(mouse.LeftIsPressed() && !mouse_tracker.Engaged())
		{
			mouse_tracker.Engage({pair.first, pair.second});
		}
		if(!mouse.LeftIsPressed())
		{
			mouse_tracker.Release();
		}

		if(mouse_tracker.Engaged())
		{
			const auto delta = mouse_tracker.Move({pair.first, pair.second});
			cam_rot = cam_rot
				* f_tdMat4::RotationY((float)-delta.x * htrack)
				* f_tdMat4::RotationX((float)-delta.y * vtrack);

		}

		m_camera_theta_y = 0;
		m_camera_theta_x = 0;
	}

	void Draw() override
	{

		phong_point_pip.BeginFrame();

		const auto view = f_tdMat4::Translation(-cam_pos) * cam_rot;

		phong_point_pip.effect.vs.BindWorldView
		(
			f_tdMat4::RotationX(m_main_model_theta_x)
			*
			f_tdMat4::RotationY(m_main_model_theta_y)
			*
			f_tdMat4::RotationZ(m_main_model_theta_y)
			*
			f_tdMat4::Scaling(1.0f)
			*
			f_tdMat4::Translation(main_mod_pos)
		);
		//std::thread th1(&PhongPointPip::Draw, std::ref(phong_point_pip));
		phong_point_pip.effect.vs.BindView(view);
		phong_point_pip.effect.vs.BindProjection(proj);
		phong_point_pip.effect.ps.SetLightPosition(light_pos);

		phong_point_pip.Draw(m_main_model);

		for(WallObj &wall : walls)
		{
			light_texture_pip.effect.vs.BindWorldView(wall.translation);
			light_texture_pip.effect.vs.BindView(view);
			light_texture_pip.effect.vs.BindProjection(proj);
			light_texture_pip.effect.vs.SetLightPosition(light_pos);
			light_texture_pip.effect.ps.bind_texture(wall.surface);
			light_texture_pip.Draw(wall.model);
		}

		light_ripple_tex_pip.effect.vs.BindWorldView(f_tdMat4::Translation(0.0f, -0.5f, 2.0f ) * f_tdMat4::RotationX(0.25f));
		light_ripple_tex_pip.effect.vs.BindView(view);
		light_ripple_tex_pip.effect.vs.BindProjection(proj);
		light_ripple_tex_pip.effect.ps.SetLightPosition(light_pos);
		light_ripple_tex_pip.effect.ps.bind_texture(&m_tex_middle_flag);
		light_ripple_tex_pip.Draw(m_flag_plain_model);

		light_ripple_tex_pip.effect.vs.BindWorldView(f_tdMat4::Translation(1.4f, -0.5f, 2.0f) * f_tdMat4::RotationX(0.25f) * f_tdMat4::RotationY(0.25f));
		light_ripple_tex_pip.effect.vs.BindProjection(proj);
		light_ripple_tex_pip.effect.ps.bind_texture(&m_tex_right_flag);
		light_ripple_tex_pip.Draw(m_flag_plain_model);

		light_ripple_tex_pip.effect.vs.BindWorldView(f_tdMat4::Translation(-1.4f, -0.5f, 2.0f) * f_tdMat4::RotationX(0.25f) * f_tdMat4::RotationY(-0.25f));
		light_ripple_tex_pip.effect.vs.BindProjection(proj);
		light_ripple_tex_pip.effect.ps.bind_texture(&m_tex_left_flag);
		light_ripple_tex_pip.Draw(m_flag_plain_model);

		light_indicator_pip.effect.vs.BindWorldView(f_tdMat4::Translation(light_pos) * view);
		light_indicator_pip.effect.vs.BindProjection(proj);
		light_indicator_pip.Draw(m_light_model);

	}

private:
	struct WallObj
	{
		const Surface* surface;
		IndexedTriangleList<LightTextureEffect::Vertex> model;
		f_tdMat4 translation;
	};

private:
	std::shared_ptr<ZBuffer> pZb;

	IndexedTriangleList<PhongPointPip::Vertex> m_main_model;
	IndexedTriangleList<SolidColorEffect::Vertex> m_light_model;
	IndexedTriangleList<LightTextureEffect::Vertex> m_wall_plain_model;
	IndexedTriangleList<LightRippleTextureEffect::Vertex> m_flag_plain_model;

	PhongPointPip phong_point_pip;
	LightIndicatorPip light_indicator_pip;
	LightTexturePip light_texture_pip;
	LightRippleTexPip light_ripple_tex_pip;

	//camera attrib
	MouseTracker mouse_tracker;
	static constexpr float hfov = 95.0f;
	static constexpr float cam_speed = 1.0f;
	Vec3 cam_pos = { 0.0f, 0.0f, 0.0f };
	f_tdMat4 cam_rot = f_tdMat4::Identity();
	float m_camera_theta_x = 0;
	float m_camera_theta_y = 0;
	float aspect_ratio = 1.33333f;
	float htrack = MyMath::to_rad(hfov) / (float)Graphics::widthR;
	float vtrack = MyMath::to_rad(hfov / aspect_ratio) / (float)Graphics::heightR;

	const f_tdMat4 proj = f_tdMat4::ProjectionHFOV(hfov, aspect_ratio, 0.1f, 10.0f);

	Vec3 main_mod_pos = { 0.0f, 1.75f, 2.0f };
	Vec3 light_pos = { 0.0f, 0.0f, 0.0f };

	float m_main_model_theta_x = MyMath::PI * 0.85f;
	float m_main_model_theta_y = 0.0f;
	float m_main_model_theta_z = 0.0f;

	std::vector<WallObj> walls;

	const Surface m_tex_floor = Surface::from_file(std::filesystem::current_path().wstring() + L"\\src\\Textures\\floortex.jpg");
	const Surface m_tex_wall = Surface::from_file(std::filesystem::current_path().wstring() + L"\\src\\Textures\\wall.jpg");
	const Surface m_tex_middle_flag = Surface::from_file(std::filesystem::current_path().wstring() + L"\\src\\Textures\\lgbt.png");
	const Surface m_tex_right_flag = Surface::from_file(std::filesystem::current_path().wstring() + L"\\src\\Textures\\mojavlast.png");
	const Surface m_tex_left_flag = Surface::from_file(std::filesystem::current_path().wstring() + L"\\src\\Textures\\mojidol.jpg");

	float game_time = 0.0f;
};