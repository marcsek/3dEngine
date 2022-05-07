#include "Graphics.h"
#include <iostream>

Graphics::Graphics()

{}

Graphics::~Graphics()
{
}

void Graphics::onCreate()
{}

void Graphics::onUpdate(HDC hdc, int width, int height, void* memory, BITMAPINFO bitmap_info, Keyboard& kbd, Mouse& mouse)
{
	Graphics::hdcR = hdc;
	Graphics::widthR = width /*/ 4*/;
	Graphics::heightR = height /*/ 4*/;
	Graphics::actualWidth = width;
	Graphics::actualHeight = height;
	Graphics::memoryR = memory;
	Graphics::bitmap_infoR = bitmap_info;
	Graphics::kbd = &kbd;
	Graphics::mouse = &mouse;
}

void Graphics::onDestroy()
{
	Window::onDestroy();
}

void Graphics::PutPixel(int in_x, int in_y, unsigned int color)
{
	/*for (int y = in_y * 4 - 2; y < in_y * 4 + 1; y++)
	{
		if (in_y + 1 < Graphics::heightR && in_y > 0 && in_x + 1 < Graphics::widthR && in_x - 2 > 0)
		{
			if((in_x * 4) - 2 < actualWidth && (in_x * 4) - 2 > 0 && y < actualHeight && y > 0 && (in_x * 4) + 1 < actualWidth && (in_x * 4) + 1 > 0)
			{
				unsigned int* pixel = (unsigned int*)memoryR + (in_x * 4) - 2 + (y * actualWidth);*/
				unsigned int* pixel = (unsigned int*)memoryR + in_x  + (in_y * actualWidth);
				//for (int x = 0; x < 3; x++)
				//{
					*pixel++ = color;
	//			}
	//		}
	//	}
	//}
}

void Graphics::DrawLine(Vec3 p1, Vec3 p2, unsigned int color)
{
	float slope = 0.0f;

	if (p1.x != p2.x)
	{
		slope = (float)(p2.y - p1.y) / (float)(p2.x - p1.x);
	}

	if (p2.x != p1.x && std::abs(slope) <= 1.0f)
	{

		if (p1.x > p2.x)
		{
			std::swap(p1.x, p2.x);
			std::swap(p1.y, p2.y);
		}

		for (int x = (int)p1.x; x <= (int)p2.x; x++)
		{
			const float y = slope * ((float)x - p1.x) + p1.y;

			PutPixel( (int)x, (int)y, color);
		}
	}
	else
	{
		if (p1.y > p2.y)
		{
			std::swap(p1.x, p2.x);
			std::swap(p1.y, p2.y);
		}

		slope = (float)(p2.x - p1.x) / (float)(p2.y - p1.y);
		for (int y = (int)p1.y; y < (int)p2.y; y++)
		{
			const float x = slope * ((float)y - p1.y) + p1.x;
			
			PutPixel( (int)x,(int)y, color);
		}
	}
}

void Graphics::ClearFrame(unsigned int color)
{
	for (int y = 0; y < actualHeight; y++)
	{
		for (int x = 0; x < actualWidth; x++)
		{
			unsigned int* pixel = (unsigned int*)memoryR + x + y * actualWidth;
			*pixel = color;
		}
	}
}

void Graphics::ComposeFrame()
{
	StretchDIBits(
		hdcR,
		0,
		0,
		actualWidth,
		actualHeight,
		0,
		0,
		actualWidth,
		actualHeight,
		memoryR,
		&bitmap_infoR,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

