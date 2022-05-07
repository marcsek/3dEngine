#pragma once
#include <vector>
#include "vendor/Vec3.h"
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"
#include "Color.h"

class Cube
{

public:
	template<class V>
	static IndexedTriangleList<V> GetSkinned(float size = 1.0f)
	{
		const float side = size / 2.0f;
		
		std::vector<Vec3> verts;

		verts.emplace_back( -side, -side, -side);
		verts.emplace_back(  side, -side, -side);
		verts.emplace_back( -side,  side, -side);
		verts.emplace_back(  side,  side, -side);	
		verts.emplace_back( -side, -side,  side);
		verts.emplace_back(  side, -side,  side);
		verts.emplace_back( -side,  side,  side);
		verts.emplace_back(  side,  side,  side);


		std::vector<V> vertices(verts.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i].pos = verts[i];
		}

		return
		{
			std::move(vertices), 
			{
				0,2,1,  2,3,1,
				1,3,5,  3,7,5,
				2,6,3,  3,6,7,
				4,5,7,  4,7,6,
				0,4,2,  2,4,6,
				0,1,4,  1,5,4
			}
		};
	}

	template<class V>
	static IndexedTriangleList<V> GetSkinnedIndependent( float size = 1.0f )
	{
		const float side = size / 2.0f;

		std::vector<Vec3> verts;

		verts.emplace_back(-side, -side, -side);
		verts.emplace_back( side, -side, -side); 
		verts.emplace_back(-side,  side, -side);
		verts.emplace_back( side,  side, -side); 
		verts.emplace_back(-side, -side,  side); 
		verts.emplace_back( side, -side,  side); 
		verts.emplace_back(-side,  side,  side); 
		verts.emplace_back( side,  side,  side); 
		verts.emplace_back(-side, -side, -side);
		verts.emplace_back(-side,  side, -side); 
		verts.emplace_back(-side, -side,  side); 
		verts.emplace_back(-side,  side,  side); 
		verts.emplace_back( side, -side, -side); 
		verts.emplace_back( side,  side, -side); 
		verts.emplace_back( side, -side,  side); 
		verts.emplace_back( side,  side,  side);
		verts.emplace_back(-side, -side, -side);
		verts.emplace_back( side, -side, -side); 
		verts.emplace_back(-side, -side,  side); 
		verts.emplace_back( side, -side,  side);
		verts.emplace_back(-side,  side, -side); 
		verts.emplace_back( side,  side, -side); 
		verts.emplace_back(-side,  side,  side); 
		verts.emplace_back( side,  side,  side); 




		std::vector<V> vertices(verts.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i].pos = verts[i];
		}

		return
		{
			std::move(vertices),
			{
				 0,  2,  1,   2,  3,  1,
				 4,  5,  7,   4,  7,  6,
				 8, 10,  9,  10, 11,  9,
				12, 13, 15,  12, 15, 14,
				16, 17, 18,  18, 17, 19,
				20, 23, 21,  20, 22, 23
			}
		};
	}

	template<class V>
	static IndexedTriangleList<V> GetIndependentFacesNormals( float size = 1.0f )
	{
		IndexedTriangleList<V> cube = GetSkinnedIndependent<V>(size);

		std::vector<Vec3> vertices;

		cube.vertices[0].n =  { 0.0f, 0.0f, -1.0f };
		cube.vertices[1].n =  { 0.0f, 0.0f, -1.0f };
		cube.vertices[2].n =  { 0.0f, 0.0f, -1.0f };
		cube.vertices[3].n =  { 0.0f, 0.0f, -1.0f };

		cube.vertices[4].n =  { 0.0f, 0.0f, 1.0f };
		cube.vertices[5].n =  { 0.0f, 0.0f, 1.0f };
		cube.vertices[6].n =  { 0.0f, 0.0f, 1.0f };
		cube.vertices[7].n =  { 0.0f, 0.0f, 1.0f };

		cube.vertices[8].n =  { -1.0f, 0.0f, 0.0f };
		cube.vertices[9].n =  { -1.0f, 0.0f, 0.0f };
		cube.vertices[10].n = { -1.0f, 0.0f, 0.0f };
		cube.vertices[11].n = { -1.0f, 0.0f, 0.0f };

		cube.vertices[12].n = { 1.0f, 0.0f, 0.0f };
		cube.vertices[13].n = { 1.0f, 0.0f, 0.0f };
		cube.vertices[14].n = { 1.0f, 0.0f, 0.0f };
		cube.vertices[15].n = { 1.0f, 0.0f, 0.0f };

		cube.vertices[16].n = { 0.0f, -1.0f, 0.0f };
		cube.vertices[17].n = { 0.0f, -1.0f, 0.0f };
		cube.vertices[18].n = { 0.0f, -1.0f, 0.0f };
		cube.vertices[19].n = { 0.0f, -1.0f, 0.0f };

		cube.vertices[20].n = { 0.0f, 1.0f, 0.0f };
		cube.vertices[21].n = { 0.0f, 1.0f, 0.0f };
		cube.vertices[22].n = { 0.0f, 1.0f, 0.0f };
		cube.vertices[23].n = { 0.0f, 1.0f, 0.0f };

		return cube;
	}
};