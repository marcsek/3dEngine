#pragma once
#include "vendor/Vec3.h"

class Color
{
public:
	unsigned int hexValue;
	Vec3 rgb;

	Color() = default;

	Color(unsigned int hexColor)
		:hexValue(hexColor),
		rgb(GetVector(hexColor))
	{}

	Color(Vec3 rgb)
		:rgb(rgb),
		hexValue(GetHexVec3(rgb))
	{}

public:
	template<class InputVer>
	static unsigned int GetHex(const InputVer& RGBColor)
	{
		int r = (int)RGBColor.color.x;
		int g = (int)RGBColor.color.y;
		int b = (int)RGBColor.color.z;

		return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
	}

	static unsigned int GetHexVec3(const Vec3& RGBColor)
	{
		int r = (int)RGBColor.x;
		int g = (int)RGBColor.y;
		int b = (int)RGBColor.z;

		return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
	}

	static Vec3 GetVector(unsigned int HEXColor)
	{
		Vec3 RGBColor;
		RGBColor.x = (float) ((HEXColor >> 16) & 0xFF);
		RGBColor.y = (float) ((HEXColor >> 8) & 0xFF);
		RGBColor.z = (float) ((HEXColor) & 0xFF);    

		return RGBColor;
	}
};

namespace Colors
{
	static Color White   = 0xFFFFFF;
	static Color Black   = 0x000000;
	static Color Gray    = 0x808080;
	static Color Silver  = 0xC0C0C0;
	static Color Red     = 0xFF0000;
	static Color Green   = 0x008000;
	static Color Blue    = 0x0000FF;
	static Color Yellow  = 0xFFFF00;
	static Color Cyan    = 0x00FFFF;
	static Color Magenta = 0xFF00FF;

}
