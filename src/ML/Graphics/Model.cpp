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
		for (Mesh *& elem : m_meshes) { delete elem; }
		m_meshes.clear();
		return m_meshes.empty();
	}

	bool Model::loadFromFile(const String & filename)
	{
		Assimp::Importer ai;
		
		const aiScene * scene { ai.ReadFile(filename.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords
		) };
		
		if (!scene) { return false; }

		// Meshes
		for (aiMesh ** m = &scene->mMeshes[0]; m != &scene->mMeshes[scene->mNumMeshes]; m++)
		{
			Mesh * temp { new Mesh() };
			
			// Faces
			for (aiFace * f = &(*m)->mFaces[0]; f != &(*m)->mFaces[(*m)->mNumFaces]; f++)
			{
				// Indices
				for (uint32_t * i = &f->mIndices[0]; i != &f->mIndices[f->mNumIndices]; i++)
				{
					const aiVector3D * vp { (*m)->mVertices ? &(*m)->mVertices[*i] : nullptr };
					const aiVector3D * vn { (*m)->mNormals ? &(*m)->mNormals[*i] : nullptr };
					const aiVector3D * uv { (*m)->HasTextureCoords(0) ? &(*m)->mTextureCoords[0][*i] : nullptr };

					temp->addVertex(Vertex {
						vp ? vec3 { vp->x, vp->y, vp->z } : vec3::zero(),
						vn ? vec4 { vn->x, vn->y, vn->z, 1.0f } : vec4::one(),
						uv ? vec2 { uv->x, uv->y } : vec2::one()
					});
				}
			}
			
			temp->setLayout(BufferLayout::get_default());
			temp->create();
			m_meshes.push_back(temp);
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

	void Model::draw(const RenderTarget & target, RenderBatch batch) const
	{
		for (const Mesh * elem : m_meshes)
		{
			elem->draw(target, batch);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}