#pragma once

class Scene
{
public:
	virtual ~Scene() = default;
	virtual void Update(Keyboard& kbd, Mouse& mouse) = 0;
	virtual void Draw() = 0;
	virtual void Reset() = 0;
};