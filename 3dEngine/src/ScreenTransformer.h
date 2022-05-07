#pragma once
#include "Graphics.h"

class ScreenTransformer
{

public:
	ScreenTransformer()
		: 
		xFactor(float( Graphics::widthR ) / 2.0f),
		yFactor(float( Graphics::heightR ) / 2.0f)
	{}

	template<class Vertex>
	Vertex& Transform(Vertex& v) const
	{
		const float wInv = 1.0f / v.pos.w;

		v *= wInv;

		v.pos.x = ( v.pos.x + 1.0f) * xFactor;
		v.pos.y = (-v.pos.y + 1.0f) * yFactor;

		v.pos.w = wInv;

		return v;
	}

	template<class Vertex>
	Vertex GetTransformed(Vertex &v) const
	{
		return Transform( Vertex(v) );
	}

private:
	float xFactor;
	float yFactor;
};