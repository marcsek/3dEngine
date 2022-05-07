#include "Game.h"
//#include "CubeBlendColorScene.h"
//#include "CubeSolidColorScene.h"
//#include "CubeFaceColorScene.h"
//#include "CubeLightShaderScene.h"
//#include "WaveVertexScene.h"
//#include "CubeSolidGeometryScene.h"
//#include "ObjModelLightScene.h"
#include "scenes/GouraudPointScene.h"
#include "Sphere.h"
#include "Input.h"
#include "TestTriangle.h"
#include "iostream"
#include "string"

Game::Game(Graphics& gfx)
	:gfx(gfx)
{
	scenes.push_back(std::make_unique<GouraudPointScene>(gfx));
	//scenes.push_back(std::make_unique<GouraudPointScene>(gfx, TestTriangle::GetNormals<GouraudPointScene::Vertex>()));

	scenesItr = scenes.begin();
}

void Game::UpdateModel()
{
	InterpolateScenes();
	(*scenesItr)->Update(*gfx.kbd, *gfx.mouse);
}

void Game::DrawModel()
{
	//long long i = 0;
	//while(true)
	//{
		//gfx.ClearFrame(0x00000);
		//i++;

		//if(i<10)
		//{
			(*scenesItr)->Draw();
			//std::cout << "kokot" << std::endl;

			//gfx.ComposeFrame();
		//}
	//}
}

void Game::InterpolateScenes()
{
	for (int i = 1; i<=9; i++)
	{
		if (Input::CheckInput('0' + i))
		{
			if (i <= scenes.size())
			{
				//scenesItr->reset(new CubeColorScene(gfx));
				scenesItr = scenes.begin() + (i - 1);
			}
		}
	}
	if (Input::CheckInput('R'))
	{
		(*scenesItr)->Reset();
	}
}