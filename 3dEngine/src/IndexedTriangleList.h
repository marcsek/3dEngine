#pragma once
#include <vector>
#include <cassert>
#include <cctype>
#include <fstream>
#include "vendor/tinyobjectloader.h"
#include "vendor/Miniball.h"


template<class V>
struct IndexedTriangleList
{
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<V> verts_in, std::vector<size_t> indicies_in)
		:
		vertices(std::move(verts_in)),
		indicies(std::move(indicies_in))
	{
		assert(vertices.size() > 2);
		assert(indicies.size() % 3 == 0);
		
		//cullFlags.resize(indicies.size() / 3, false);
	}

	static IndexedTriangleList<V> load(const std::string& filename)
	{
		IndexedTriangleList<V> tl;

		bool isCCW = false;
		{
			std::ifstream file(filename);
			std::string firstline;
			std::getline(file, firstline);
			std::transform(firstline.begin(), firstline.end(), firstline.begin(), std::tolower);

			if(firstline.find("ccw") != std::string::npos)
			{
				isCCW = true;
			}
		}

		using namespace tinyobj;
		attrib_t attrib;
		std::vector<shape_t> shapes;
		std::string err;
		std::string wrn;

		const bool ret = LoadObj(&attrib, &shapes, nullptr, &wrn, &err, filename.c_str());

		if(!err.empty() && err.substr(0,4) != "WARN")
		{
			throw std::runtime_error(("Load returned error: " + err).c_str());
		}
		if (!ret)
		{
			throw std::runtime_error(("Load false file: " + filename).c_str());
		}
		if (shapes.size() == 0u)
		{
			throw std::runtime_error(("No shapes could be loaded File: " + filename).c_str());
		}

		tl.vertices.reserve(attrib.vertices.size() / 3u);
		for (int i = 0; i < attrib.vertices.size(); i+= 3)
		{
			tl.vertices.emplace_back
			(
				Vec3
				{
					attrib.vertices[i + 0],
					attrib.vertices[i + 1],
					attrib.vertices[i + 2],
				}
			);
		}

		const auto& mesh = shapes[0].mesh;
		tl.indicies.reserve(mesh.indices.size());
		for(size_t f = 0; f < mesh.num_face_vertices.size(); f++)
		{
			if(mesh.num_face_vertices[f] != 3u)
			{
				std::stringstream ss;
				ss << "Load error face #" << f << " has " << mesh.num_face_vertices[f] << " vertices";

				throw std::runtime_error(ss.str().c_str());
			}

			for(size_t vn = 0; vn < 3u; vn++)
			{
				const auto idx = mesh.indices[f * 3u + vn];
				tl.indicies.push_back(size_t(idx.vertex_index));
		
				tl.vertices[(size_t)idx.vertex_index].n = Vec3
				{
					attrib.normals[3 * idx.normal_index + 0],
					attrib.normals[3 * idx.normal_index + 1],
					attrib.normals[3 * idx.normal_index + 2]
				};
			}

			if(isCCW)
			{
				std::swap(tl.indicies.back(), *std::prev(tl.indicies.end(), 2));
			}
		}

		return tl;
	}

	void AdjustToTrueCenter()
	{
		struct VertexAccessor
		{
			typedef std::vector<V>::const_iterator Pit;
			typedef const float* Cit;
			Cit operator()(Pit it) const
			{
				return &it->pos.x;
			}
		};

		Miniball::Miniball<VertexAccessor> mb(3, vertices.cbegin(), vertices.cend());
		const auto pc = mb.center();
		const Vec3 center = { *pc, *std::next(pc), *std::next(pc,2) };

		for(auto& v : vertices)
		{
			v.pos -= center;
		}
	}

	float GetRadius() const
	{
		return std::max_element(vertices.cbegin(), vertices.cend(),
			[](const V& v0, const V& v1)
			{
				return v0.pos.LenSq() < v1.pos.LenSq();
			}
		)->pos.Len();
	}
	std::vector<V> vertices;
	std::vector<size_t> indicies;
};
