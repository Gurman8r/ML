#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/Geometry.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/scene.h>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Model::Model()
		: m_meshes {}
	{
	}

	Model::Model(const String & filename)
		: Model {}
	{
		loadFromFile(filename);
	}

	Model::Model(const Model & copy)
		: Model {}
	{
		m_meshes.reserve(copy.meshes().size());
		for (const Mesh * mesh : copy.meshes())
		{
			m_meshes.push_back(new Mesh { *mesh });
			m_meshes.back()->create();
		}
	}

	Model::~Model() { dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Model::dispose()
	{
		for (Mesh *& elem : m_meshes)
		{
			delete elem;
		}
		m_meshes.clear();
		return m_meshes.empty();
	}

	bool Model::loadFromFile(const String & filename)
	{
		Assimp::Importer _importer;
		const aiScene * scene { _importer.ReadFile(filename.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType
		) };
		if (!scene) return false;
		for (uint32_t m = 0; m < scene->mNumMeshes; m++)
		{
			const aiMesh * mesh { scene->mMeshes[m] };
			for (uint32_t f = 0; f < mesh->mNumFaces; f++)
			{
				Mesh * temp { new Mesh() };
				const aiFace & face = mesh->mFaces[f];
				for (uint32_t i = 0; i < 3; i++)
				{
					const aiVector3D * v { &mesh->mVertices[face.mIndices[i]] };
					const aiVector3D * n { &mesh->mVertices[face.mIndices[i]] };
					const aiVector3D * t { mesh->HasTextureCoords(0)
						? &mesh->mTextureCoords[0][face.mIndices[i]]
						: nullptr 
					};

					temp->addVertex(Vertex {
						(v ? vec3 { v->x, v->y, v->z } : vec3 { NULL }),
						(n ? vec4 { n->x, n->y, n->z, 1.0f } : vec4 { NULL }),
						(t ? vec2 { t->x, t->y } : vec2 { NULL })
					});
				}
				temp->setLayout(BufferLayout::get_default());
				temp->create();
				m_meshes.push_back(temp);
			}
		}
		return !m_meshes.empty();
	}

	bool Model::loadFromMemory(const List<Vertex> & vertices)
	{
		if (dispose())
		{
			m_meshes.push_back(new Mesh { vertices, {}, {} });
		}
		return !m_meshes.empty();
	}

	bool Model::loadFromMemory(const List<Vertex> & vertices, const List<uint32_t> & indices)
	{
		if (dispose())
		{
			m_meshes.push_back(new Mesh { vertices, indices, {} });
		}
		return !m_meshes.empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Model::draw(RenderTarget & target, RenderBatch batch) const
	{
		for (const Mesh * elem : m_meshes)
		{
			elem->draw(target, batch);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}