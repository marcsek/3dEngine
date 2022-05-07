#include "Window.h"
#include <iostream>

int Window::height = 0;
int Window::width = 0;
unsigned int Window::backgorund_color = 0x000000;
void* Window::memory = nullptr;
BITMAPINFO Window::bitmap_info;
HDC Window::hdc = nullptr;
Window* windowObj = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		windowObj->onCreate();
		break;
	}
	case WM_DESTROY:
	{
		windowObj->onDestroy();
		::PostQuitMessage(0);
		break;
	}

	case WM_KEYDOWN:
		windowObj->kbd.SetKeyPressed(static_cast<unsigned char>(wparam));
		break;

	case WM_KEYUP:
		windowObj->kbd.SetKeyReleased(static_cast<unsigned char>(wparam));
		break;

	case WM_KILLFOCUS:
		windowObj->kbd.ClearState();
		break;

	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lparam);
		if(pt.x > 0 && pt.x < windowObj->width && pt.y > 0 && pt.y < windowObj->height)
		{
			windowObj->mouse.OnMouseMove(pt.x, pt.y);
		}
		break;
	}

	case WM_LBUTTONDOWN:
		windowObj->mouse.OnLeftPress();
		break;

	case WM_LBUTTONUP:
		windowObj->mouse.OnLeftRelease();
		break;

	case WM_RBUTTONDOWN:
		windowObj->mouse.OnRightPress();
		break;

	case WM_RBUTTONUP:
		windowObj->mouse.OnRightRelease();
		break;

	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		windowObj->width = rect.right - rect.left;
		windowObj->height = rect.bottom - rect.top;

		int size = windowObj->width * windowObj->height * sizeof(unsigned int);

		if (windowObj->memory) VirtualFree(windowObj->memory, 0, MEM_RELEASE);
		windowObj->memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		windowObj->bitmap_info.bmiHeader.biSize = sizeof(windowObj->bitmap_info.bmiHeader);
		windowObj->bitmap_info.bmiHeader.biWidth = windowObj->width;
		windowObj->bitmap_info.bmiHeader.biHeight = windowObj->height;
		windowObj->bitmap_info.bmiHeader.biPlanes = 1;
		windowObj->bitmap_info.bmiHeader.biBitCount = 32;
		windowObj->bitmap_info.bmiHeader.biCompression = BI_RGB;

		windowObj->onUpdate(windowObj->hdc, windowObj->width, windowObj->height, windowObj->memory, windowObj->bitmap_info, windowObj->kbd, windowObj->mouse);
		break;
	}
	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

bool Window::init()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;
	
	if (!::RegisterClassEx(&wc)) {
		return false;
	}

	if (!windowObj)
		windowObj = this;

	window=::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"OOP", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, NULL, NULL);

	hdc = GetDC(window);
	if (!window)
		return false;

	::ShowWindow(window, SW_SHOW);
	::UpdateWindow(window);

	m_is_run = true;

	hdc = GetDC(window);

	return true;
}

bool Window::broadcast(float fps)
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)>0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	std::stringstream sstream;
	sstream << fps;
	std::string fpsString ="OOP | FPS: " +sstream.str();
	std::wstring stemp = std::wstring(fpsString.begin(), fpsString.end());
	LPCWSTR sw = stemp.c_str();

	::SetWindowText(window, sw);
	return true;
}

bool Window::release()
{
	if (!::DestroyWindow(window))
		return false;

	return true;
}


bool Window::isRun()
{
	return m_is_run;
}

void Window::onUpdate(HDC hdc, int width, int height, void* memory, BITMAPINFO bitmap_info, Keyboard& kbd, Mouse& mouse)
{
}

void Window::onDestroy()
{
	m_is_run = false;
}

Window::~Window()
{

}