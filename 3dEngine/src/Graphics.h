#pragma once
#include "Window.h"
#include <Windows.h>
#include "vendor/Vec3.h"
#include <memory>

class Graphics : public Window
{
public:
	Graphics();
	~Graphics();
	
	virtual void onCreate() override;
	virtual void onUpdate(HDC hdc, int width, int height, void* memory, BITMAPINFO bitmap_info, Keyboard& kbd, Mouse& mouse) override;
	virtual void onDestroy() override;

	void PutPixel(int x, int y, unsigned int color);
	void DrawLine(Vec3 p1, Vec3 p2, unsigned int color);
	//void DrawTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, unsigned int c);

	void ClearFrame(unsigned int color);
	void ComposeFrame();

//private:
//	void DrawTraingleFlatTop(const Vec2& v0, const Vec2& v1, const Vec2& v2, unsigned int color);
//	void DrawTraingleFlatBot(const Vec2& v0, const Vec2& v1, const Vec2& v2, unsigned int color);

public:
	inline static int widthR;
	inline static int heightR;
	inline static int actualWidth;
	inline static int actualHeight;
	inline static void* memoryR;
	Keyboard* kbd;
	Mouse* mouse;

private:
	HDC hdcR;
	BITMAPINFO bitmap_infoR;
};

