#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/Geometry.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/scene.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline Mesh * processMesh(aiMesh * mesh, const aiScene * scene)
	{
		Mesh * temp { new Mesh() };

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			temp->addVertex(Vertex {
				mesh->mVertices
					? vec3 { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z }
					: vec3 { uninit },
				mesh->mNormals
					? vec4 { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 1.0f }
					: vec4 { uninit },
				mesh->mTextureCoords[0]
					? vec2 { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y }
					: vec2 { uninit }
			});
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace & face = mesh->mFaces[i];

			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				temp->addIndex(face.mIndices[j]);
			}
		}

		temp->setLayout(BufferLayout::get_default());

		return &temp->create();
	}

	static inline void processNode(List<Mesh *> & meshes, aiNode * node, const aiScene * scene)
	{
		// process all of the node's meshes and then do the same for each of its children
		if (node && scene)
		{
			for (uint32_t i = 0; i < node->mNumMeshes; i++)
			{
				if (aiMesh * mesh = scene->mMeshes[node->mMeshes[i]])
				{
					meshes.push_back(processMesh(mesh, scene));
				}
			}

			for (uint32_t i = 0; i < node->mNumChildren; i++)
			{
				if (aiNode * child { node->mChildren[i] })
				{
					processNode(meshes, child, scene);
				}
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Model::Model()
		: m_meshes {}
	{
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
		if (const aiScene * scene = aiImportFile(filename.c_str(), aiProcess_Triangulate))
		{
			if (!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode)
			{
				processNode(m_meshes, scene->mRootNode, scene);

				return !m_meshes.empty();
			}
		}
		return false;
	}

	bool Model::loadFromMemory(const List<Vertex>& vertices)
	{
		if (dispose())
		{
			m_meshes.push_back(new Mesh { vertices, {}, {} });
		}
		return !m_meshes.empty();
	}

	bool Model::loadFromMemory(const List<Vertex>& vertices, const List<uint32_t>& indices)
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
			target.draw(elem);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}