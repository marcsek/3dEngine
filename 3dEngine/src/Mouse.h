#pragma once
#include <utility>


class Mouse
{
public:
	std::pair<int, int> GetPos() const
	{
		return { x,y };
	}

	void OnMouseMove(int newx, int newy)
	{
		x = newx;
		y = newy;
	}

	void OnRightPress()
	{
		rightIsPressed = true;
	}

	void OnRightRelease()
	{
		rightIsPressed = false;
	}

	void OnLeftPress()
	{
		leftIsPressed = true;
	}

	void OnLeftRelease()
	{
		leftIsPressed = false;
	}

	bool LeftIsPressed()
	{
		return leftIsPressed;
	}

	bool RightIsPressed()
	{
		return rightIsPressed;
	}

private:
	int x, y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
};
