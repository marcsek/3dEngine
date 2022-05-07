#pragma once

#include <limits>
#include <cassert>
#include <memory>
#include <algorithm>

class ZBuffer
{
public:
	ZBuffer() = default;
	ZBuffer(int width, int height)
		: width(width),
		height(height),
		pBuffer(std::make_unique<float[]>(width * height))
	{}

	ZBuffer(const ZBuffer&) = delete;
	ZBuffer& operator=(const ZBuffer&) = delete;

	void Clear()
	{
		for (int i = 0; i < width * height; i++)
		{
			pBuffer[i] = std::numeric_limits<float>::infinity();
		}
	}

	float& At(int x, int y)
	{
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);

		return pBuffer[y * width + x];
	}

	const float& At(int x, int y) const
	{
		return const_cast<ZBuffer*>(this)->At(x, y);
	}

	bool TestAndSet(int x, int y, float depth)
	{
		float& depthInBuffer = At(x, y);
		if (depth < depthInBuffer)
		{
			depthInBuffer = depth;
			return true;
		}
		return false;
	}


private:
	int width, height;
	std::unique_ptr<float[]> pBuffer = nullptr;
};