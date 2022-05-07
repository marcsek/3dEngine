#pragma once

#define FULL_WINTARD
#include "Windows.h"
#include "Surface.h"
#include <memory>

namespace Gdiplus
{
	using std::min;
	using std::max;
}

#include <cassert>
#include <gdiplus.h>
#include <sstream>
#include "Color.h"

class Surface
{
public:
	Surface(unsigned int width, unsigned int height, unsigned int pitch)
		:pixels_buffer_m(std::make_unique<Color[]>(width * height)),
		width_m_(width),
		height_m_(height),
		pitch_m_(pitch)
	{}

	Surface(Surface&& source) 
		:
		pixels_buffer_m(std::move(source.pixels_buffer_m)),
		width_m_(source.width_m_),
		height_m_(source.height_m_),
		pitch_m_(source.pitch_m_)
	{}

private:
	Surface(unsigned int width, unsigned int height, unsigned int pitch, std::unique_ptr<Color[]> pixels_buffer_p)
		:pixels_buffer_m(std::move(pixels_buffer_p)),
		width_m_(width),
		height_m_(height),
		pitch_m_(pitch)
	{}

public:

	unsigned int get_width() const
	{
		return width_m_;
	}

	unsigned int get_height() const
	{
		return height_m_;
	}

	Color get_pixel(unsigned int x, unsigned int y) const
	{
		if (height_m_ == y)
		{
			y = height_m_ - 1;
		}
		assert(x >= 0);
		assert(y >= 0);
		assert(x < width_m_);
		assert(y < height_m_);
		

		return pixels_buffer_m[y * pitch_m_ + x];
	}
	static Surface from_file(const std::wstring& name)
	{
		//static const wchar_t* filename = L"C:\\Users\\jakub\\source\\repos\\3dEngine\\3dEngine\\src\\Textures\\wall.jpg";

		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		std::unique_ptr<Color[]> pixels_buffer;
		unsigned int width = 0, height = 0, pitch = 0;

		{
			Gdiplus::Bitmap bitmap(name.c_str());
			if(bitmap.GetLastStatus() != Gdiplus::Status::Ok)
			{
				std::wstringstream ss;
				ss << L"Loading image [" << name << L"]: failed to load";
				assert(false);
			}

			pitch = width = bitmap.GetWidth();
			height = bitmap.GetHeight();
			pixels_buffer = std::make_unique<Color[]>(width * height);

			for(unsigned int y = 0; y < height; y++)
			{
				for (unsigned int x = 0; x < width; x++)
				{
					Gdiplus::Color c;
					bitmap.GetPixel(x, y, &c);
					pixels_buffer[((height - y) - 1) * pitch + x] = c.GetValue();
				}
			}
		}
		return Surface(width, height, pitch, std::move(pixels_buffer));
	}

private:
	std::unique_ptr<Color[]> pixels_buffer_m;
	unsigned int width_m_ = 0, height_m_ = 0, pitch_m_ = 0;
};