#pragma once
#include "Windows.h"

class GDIPManager
{
public:
	GDIPManager();
	~GDIPManager();
private:
	static ULONG_PTR token;
	static int refCount;
};