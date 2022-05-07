#pragma once
#include <Windows.h>
#include <sstream>
#include "Keyboard.h"
#include "Mouse.h"
#include <memory>

class Window
{
public :
	bool init();
	bool release();
	bool broadcast(float fps);
	bool isRun();
	~Window();

	virtual void onCreate() = 0;
	virtual void onUpdate(HDC hdc, int width, int height, void* memory, BITMAPINFO bitmap_info, Keyboard& kbd, Mouse& mouse);
	virtual void onDestroy();

	static int height, width;
	static unsigned int backgorund_color;
	static void* memory;
	static BITMAPINFO bitmap_info;
	static HDC hdc;

	Keyboard kbd;
	Mouse mouse;
protected:
	HWND window;
	bool m_is_run;
};
