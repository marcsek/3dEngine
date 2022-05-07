#pragma once
#include "../../vendor/Mat.h"

template<class Vertex>
class DefaultVertexShader
{
public:
	typedef Vertex Output;

public:
	void BindRotation(const f_tdMat3& rotation_in)
	{
		rotation = rotation_in;
	}
	void BindTranslation(const Vec3& translation_in)
	{
		translation = translation_in;
	}
	Output operator()(const Vertex& in) const
	{
		return{ in.pos * rotation + translation, in };
	}

private:
	f_tdMat3 rotation;
	Vec3 translation;
};
