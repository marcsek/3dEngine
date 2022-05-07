#pragma once
#include "Graphics.h"
#include "Scene.h"
#include <vector>
#include "GDI/GDIPManager.h"

class Game
{
public:
	Game(Graphics& gfx);

	void UpdateModel();
	void DrawModel();

private:
	Graphics gfx;

	//User vars
	std::vector<std::unique_ptr<Scene>> scenes;
	std::vector<std::unique_ptr<Scene>>::iterator scenesItr;
	void InterpolateScenes();
};

