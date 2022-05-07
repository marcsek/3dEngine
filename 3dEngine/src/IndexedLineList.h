#pragma once

#include "vendor/Vec3.h"
#include <vector>

struct IndexedLineList
{
	std::vector<Vec3> vertices;
	std::vector<size_t> indicies;
};