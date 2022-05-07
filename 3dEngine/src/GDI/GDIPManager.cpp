#define FULL_WINTARD
#include "Windows.h"
#include "GDIPManager.h"
#include <algorithm>

namespace Gdiplus
{
	using std::min;
	using std::max;
}

#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

ULONG_PTR GDIPManager::token = 0;
int GDIPManager::refCount = 0;

GDIPManager::GDIPManager()
{
	if (refCount++ == 0)
	{
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup(&token, &input, nullptr);
	}
}

GDIPManager::~GDIPManager()
{
	if (--refCount == 0)
	{
		Gdiplus::GdiplusShutdown(token);
	}
}