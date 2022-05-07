#pragma once
#include "vendor/Vec3.h"
#include "Graphics.h"
#include "IndexedTriangleList.h"
#include <vector>
#include "vendor/Mat.h"
#include "Triangle.h"
#include "ScreenTransformer.h"
#include "Vertex.h"
#include "ZBuffer.h"
#include <memory>
#include <algorithm>

template<class Effect>
class Pipeline
{

public:
	typedef typename Effect::Vertex Vertex;
	typedef typename Effect::VertexShader::Output VSout;
	typedef typename Effect::GeometryShader::Output GSout;

	Pipeline(Graphics& gfx)
		: Pipeline(gfx, std::make_shared<ZBuffer>(Graphics::widthR, Graphics::heightR))
	{}

	Pipeline(Graphics& gfx, std::shared_ptr<ZBuffer> pZb)
		: gfx(gfx),
		pZb(std::move(pZb))
	{
	}

	void Draw(IndexedTriangleList<Vertex>& trilist)
	{
		ProcessVertices(trilist.vertices, trilist.indicies);
	}

	void BeginFrame()
	{
		pZb->Clear();
	}

private:
	
	void ProcessVertices(const std::vector<Vertex>& verts, const std::vector<size_t>& indicies)
	{
		std::vector<VSout> verticesOut(verts.size());

		std::transform(verts.begin(), verts.end(), verticesOut.begin(), effect.vs);

		//for(size_t i = 0; i < verts.size(); i++)
		//{
		//	verticesOut[i] = effect.vs(verts[i]);
		//}

		//AssembleTriangles(verticesOut, indicies);
	}

	void AssembleTriangles(const std::vector<VSout>& verts, const std::vector<size_t>& indicies)
	{
		const auto eyepos = Vec4{0.0f, 0.0f, 0.0f, 1.0f} * effect.vs.GetProj();
		for (size_t i = 0, end = indicies.size() / 3; i < end; i++)
		{
			const auto& v0 = verts[indicies[i * 3 + 0]];
			const auto& v1 = verts[indicies[i * 3 + 1]];
			const auto& v2 = verts[indicies[i * 3 + 2]];
			
			if ((v1.pos - v0.pos) % (v2.pos - v0.pos) * Vec3(v0.pos - eyepos) <= 0.0f)
			{
				ProcessTriangle(v0, v1, v2, i);
			}
		}
	}

	void ProcessTriangle(const VSout& v0, const VSout& v1, const VSout& v2, size_t triangle_index)
	{
		Triangle<GSout> trianToProcess = effect.gs(v0, v1, v2, triangle_index);
		ClipCullTriangle( trianToProcess );
	}

	void ClipCullTriangle(Triangle<GSout>& t)
	{
		if (t.v0.pos.x > t.v0.pos.w &&
			t.v1.pos.x > t.v1.pos.w &&
			t.v2.pos.x > t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.x < -t.v0.pos.w &&
			t.v1.pos.x < -t.v1.pos.w &&
			t.v2.pos.x < -t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.y > t.v0.pos.w &&
			t.v1.pos.y > t.v1.pos.w &&
			t.v2.pos.y > t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.y < -t.v0.pos.w &&
			t.v1.pos.y < -t.v1.pos.w &&
			t.v2.pos.y < -t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.z > t.v0.pos.w &&
			t.v1.pos.z > t.v1.pos.w &&
			t.v2.pos.z > t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.z < 0.0f &&
			t.v1.pos.z < 0.0f &&
			t.v2.pos.z < 0.0f)
		{
			return;
		}

		// clipping routines
		const auto Clip1 = [this](GSout& v0, GSout& v1, GSout& v2)
		{
			// calculate alpha values for getting adjusted vertices
			const float alphaA = (-v0.pos.z) / (v1.pos.z - v0.pos.z);
			const float alphaB = (-v0.pos.z) / (v2.pos.z - v0.pos.z);
			// interpolate to get v0a and v0b
			const auto v0a = MyMath::interpolate(v0, v1, alphaA);
			const auto v0b = MyMath::interpolate(v0, v2, alphaB);
			// draw triangles
			Triangle<GSout> trianToProcess = { v0a, v1, v2 };
			Pipeline::PostProcessTriangleVerts(trianToProcess);
			Triangle<GSout> trianToProcess2 = { v0b, v0a, v2 };
			Pipeline::PostProcessTriangleVerts( trianToProcess2 );
		};
		const auto Clip2 = [this](GSout& v0, GSout& v1, GSout& v2)
		{
			// calculate alpha values for getting adjusted vertices
			const float alpha0 = (-v0.pos.z) / (v2.pos.z - v0.pos.z);
			const float alpha1 = (-v1.pos.z) / (v2.pos.z - v1.pos.z);
			// interpolate to get v0a and v0b
			v0 = MyMath::interpolate(v0, v2, alpha0);
			v1 = MyMath::interpolate(v1, v2, alpha1);
			// draw triangles
			Triangle<GSout> trianToProcess = { v0, v1, v2 };
			Pipeline::PostProcessTriangleVerts(trianToProcess);
		};

		// near clipping tests
		if (t.v0.pos.z < 0.0f)
		{
			if (t.v1.pos.z < 0.0f)
			{
				Clip2(t.v0, t.v1, t.v2);
			}
			else if (t.v2.pos.z < 0.0f)
			{
				Clip2(t.v0, t.v2, t.v1);
			}
			else
			{
				Clip1(t.v0, t.v1, t.v2);
			}
		}
		else if (t.v1.pos.z < 0.0f)
		{
			if (t.v2.pos.z < 0.0f)
			{
				Clip2(t.v1, t.v2, t.v0);
			}
			else
			{
				Clip1(t.v1, t.v0, t.v2);
			}
		}
		else if (t.v2.pos.z < 0.0f)
		{
			Clip1(t.v2, t.v0, t.v1);
		}
		else // no near clipping necessary
		{
			Triangle<GSout> trianToProcess(t);
			Pipeline::PostProcessTriangleVerts(trianToProcess);
		}
	}

	void PostProcessTriangleVerts(Triangle<GSout>& triangle)
	{
		st.Transform(triangle.v0);
		st.Transform(triangle.v1);
		st.Transform(triangle.v2);
		
		DrawTriangle(triangle);
	}

	void DrawTriangle(Triangle<GSout>& triangle)
	{
		const GSout* pv0 = &triangle.v0;
		const GSout* pv1 = &triangle.v1;
		const GSout* pv2 = &triangle.v2;
	
		// pv2 = najspodnejsi bod, pv0 = najvyssi bod
		if (pv1->pos.y < pv0->pos.y) std::swap(pv1, pv0);
		if (pv2->pos.y < pv1->pos.y) std::swap(pv1, pv2);
		if (pv1->pos.y < pv0->pos.y) std::swap(pv1, pv0);
	
		if (pv0->pos.y == pv1->pos.y)
		{
			if (pv1->pos.x < pv0->pos.x) std::swap(pv1, pv0);
			DrawTraingleFlatTop(*pv0, *pv1, *pv2);
		}
	
		else if (pv1->pos.y == pv2->pos.y)
		{
			if (pv2->pos.x < pv1->pos.x) std::swap(pv1, pv2);
			DrawTraingleFlatBot(*pv0, *pv1, *pv2);
		}
		else // vseobecy trojuholnik - musim poskladat z dvoch
		{
			const float alphaSplit =
				(pv1->pos.y - pv0->pos.y)
				/
				(pv2->pos.y - pv0->pos.y);
	
			const auto vi = *pv0 + (*pv2 - *pv0) * alphaSplit;
	
			if (pv1->pos.x < vi.pos.x)
			{
				DrawTraingleFlatBot(*pv0, *pv1, vi);
				DrawTraingleFlatTop(*pv1, vi, *pv2);
			}
			else
			{
				DrawTraingleFlatBot(*pv0, vi, *pv1);
				DrawTraingleFlatTop(vi, *pv1, *pv2);
			}
		}
	}
	
	void DrawTraingleFlatTop(const GSout& v0, const GSout& v1, const GSout& v2)
	{
		const float delta_y = v2.pos.y - v0.pos.y;
		const auto dit0 = (v2 - v0) / delta_y;
		const auto dit1 = (v2 - v1) / delta_y;

		auto itEdge1 = v1;

		DrawFlatTriangle(v0, v1, v2, dit0, dit1, itEdge1);
	}
	
	void DrawTraingleFlatBot(const GSout& v0, const GSout& v1, const GSout& v2)
	{

		const float delta_y = v2.pos.y - v0.pos.y;
		const auto dit0 = (v1 - v0) / delta_y;
		const auto dit1 = (v2 - v0) / delta_y;

		auto itEdge1 = v0;

		DrawFlatTriangle(v0, v1, v2, dit0, dit1, itEdge1);
	}

	void DrawFlatTriangle(const GSout& v0,
		const GSout& v1,
		const GSout& v2,
		const GSout& dv0,
		const GSout& dv1,
		GSout itEdge1
	)
	{
		auto itEdge0 = v0;
		float offset = 0.5f;

		const int yStart = (std::max)((int)ceil(v0.pos.y - offset), 0);
		const int yEnd = (std::min)((int)ceil(v2.pos.y - offset), (int)Graphics::heightR - 1);

		itEdge0 += dv0 * (float(yStart) + offset - v0.pos.y);
		itEdge1 += dv1 * (float(yStart) + offset - v0.pos.y);

		for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
		{
			const int xStart = (std::max)((int)ceil(itEdge0.pos.x - offset), 0);
			const int xEnd = (std::min)((int)ceil(itEdge1.pos.x - offset), (int)Graphics::widthR - 1);

			auto iLine = itEdge0;
			const float dx = itEdge1.pos.x - itEdge0.pos.x;
			const auto diLine = (itEdge1 - iLine) / dx;

			iLine += diLine * (float(xStart) + offset - itEdge0.pos.x);

			for (int x = xStart; x < xEnd; x++, iLine += diLine)
			{
				if (pZb->TestAndSet(x, y, iLine.pos.z))
				{
					const float w = 1.0f / iLine.pos.w;
					const auto attr = iLine * w;

					gfx.PutPixel(x, y, effect.ps(attr));
				}
			}
		}
	}


public:
	Effect effect;

private:
	Graphics gfx;
	ScreenTransformer st;
	//ZBuffer zb;
	std::shared_ptr<ZBuffer> pZb;
};
