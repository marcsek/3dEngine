#pragma once
#include <bitset>

class Keyboard
{
public:
	Keyboard()
	{
		//p_keyStates['Q'] = true;
	}

	bool KeyIsPressed(unsigned char key_code)
	{
		return p_keyStates[key_code];
	}

	void SetKeyPressed(unsigned char key_code)
	{
		p_keyStates[key_code] = true;
	}

	void SetKeyReleased(unsigned char key_code)
	{
		p_keyStates[key_code] = false;
	}

	void ClearState()
	{
		p_keyStates.reset();
	}

private:
	std::bitset<256u> p_keyStates;
};