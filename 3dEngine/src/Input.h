#pragma once
#include <windows.h>

class Input
{
public:
	static bool CheckInput(char key)
	{
		if (GetAsyncKeyState(key) & 0x8000)
		{
			return true;
		}
		return false;
	}
};